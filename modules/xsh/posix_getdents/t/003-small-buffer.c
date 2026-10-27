/*
 * A buffer of one structure plus NAME_MAX bytes still makes progress to
 * the end.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_getdents.003
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static inline void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out (a call did not return)");
}

/* Limit the whole test: a hang becomes a FAIL (cleanup runs at exit). */
static inline void guard(unsigned secs)
{
	(void)signal(SIGALRM, on_alarm);
	(void)alarm(secs);
}

static inline void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Run fn in a child; its exit status is what fn returns (0 to 96). */
static inline pid_t spawn(int (*fn)(void))
{
	pid_t k = fork();

	if (k == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (k == 0) {
		(void)signal(SIGALRM, SIG_DFL);
		(void)alarm(25);
		_exit(fn());
	}
	return k;
}

/* Exit status of k; -1 if it did not end within secs (it is killed), -2 if a signal ended it. */
static inline int reap(pid_t k, int secs)
{
	int st, i;

	for (i = 0; i < secs * 100; i++) {
		pid_t r = waitpid(k, &st, WNOHANG);

		if (r == k)
			return WIFEXITED(st) ? WEXITSTATUS(st) : -2;
		if (r == -1 && errno != EINTR)
			return -2;
		sleep_ms(10);
	}
	(void)kill(k, SIGKILL);
	(void)waitpid(k, &st, 0);
	return -1;
}

/* Whether k has not ended yet (its status stays collectable). */
static inline int running(pid_t k)
{
	siginfo_t si;

	si.si_pid = 0;
	if (waitid(P_PID, (id_t)k, &si, WEXITED | WNOHANG | WNOWAIT) != 0)
		pc_unresolved("waitid failed: %s", pc_errname(errno));
	return si.si_pid == 0;
}

/*
 * Child result for "the call must fail with err": 0 when it did, 1 when it
 * succeeded, 100 + errno for another error.
 */
static inline int want_err(long r, int err)
{
	if (r != -1)
		return 1;
	return errno == err ? 0 : 100 + (errno < 150 ? errno : 149);
}

static inline const char *child_said(int rc)
{
	static char buf[64];

	if (rc == 0)
		return "as expected";
	if (rc == 1)
		return "the call succeeded";
	if (rc >= 100) {
		snprintf(buf, sizeof buf, "failed with %s", pc_errname(rc - 100));
		return buf;
	}
	if (rc < 0)
		return "the child did not end normally or in time";
	snprintf(buf, sizeof buf, "code %d", rc);
	return buf;
}

/*
 * Run fn in a child that has no appropriate privileges: when the test runs
 * as root, the child takes user and group 65534. Returns what fn returns
 * (0 to 96); a child that cannot drop privileges makes the test UNRESOLVED.
 */
static inline int as_other(int (*fn)(void))
{
	pid_t kid;
	int st;

	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		if (geteuid() == 0) {
			(void)pc_drop_groups();
			if (setgid(65534) != 0 || setuid(65534) != 0)
				_exit(99);
		}
		if (geteuid() == 0)
			_exit(99);
		(void)signal(SIGALRM, SIG_DFL);
		(void)alarm(25);
		_exit(fn());
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st) || WEXITSTATUS(st) == 99)
		pc_unresolved("the unprivileged child could not run (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

static inline void in_range(long v, long lo, long hi, const char *what)
{
	pc_check(v >= lo && v <= hi, "%s is %ld, expected between %ld and %ld", what, v, lo, hi);
}

/* A handler that does nothing, installed without SA_RESTART. */
static inline void noop_handler(int s)
{
	(void)s;
}

static inline void catch_usr1(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = noop_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGUSR1, &sa, NULL) != 0)
		pc_unresolved("sigaction failed: %s", pc_errname(errno));
}

/* Send SIGUSR1 to k every 100 ms until it ends; its exit status as reap(). */
static inline int interrupt_until_done(pid_t k, int secs)
{
	int i, st;

	for (i = 0; i < secs * 10; i++) {
		pid_t r;

		sleep_ms(100);
		r = waitpid(k, &st, WNOHANG);
		if (r == k)
			return WIFEXITED(st) ? WEXITSTATUS(st) : -2;
		(void)kill(k, SIGUSR1);
	}
	(void)kill(k, SIGKILL);
	(void)waitpid(k, &st, 0);
	return -1;
}

#include <dirent.h>

#define MAXE 64

struct ent {
	char name[300];
	ino_t ino;
	unsigned char type;
};

static struct ent ents[MAXE];
static int nents;
static char longname[300];

/* The test directory dd: a, b, c, a name of {NAME_MAX} bytes, sub/, a link to a, and a FIFO. */
static inline int make_dd(void)
{
	long max = pathconf(".", _PC_NAME_MAX);
	char path[400];
	int fd;

	if (max < 0 || max + 1 > (long)sizeof longname - 1)
		pc_unresolved("{NAME_MAX} is indeterminate or too large here (%ld)", max);
	pc_make_dir("dd");
	pc_make_file("dd/a", "a");
	pc_make_file("dd/b", "b");
	pc_make_file("dd/c", "c");
	memset(longname, 'L', (size_t)max);
	longname[max] = '\0';
	snprintf(path, sizeof path, "dd/%s", longname);
	pc_make_file(path, "l");
	pc_make_dir("dd/sub");
	pc_make_symlink("a", "dd/ln");
	if (mkfifo("dd/ff", 0600) != 0)
		pc_unresolved("mkfifo failed: %s", pc_errname(errno));
	fd = open("dd", O_RDONLY);
	if (fd < 0)
		pc_unresolved("open(dd) failed: %s", pc_errname(errno));
	return fd;
}

/* Names readdir() lists in dd, into names (setup use); the count. */
static inline int readdir_names(char names[][300], int max)
{
	DIR *d = opendir("dd");
	struct dirent *e;
	int n = 0;

	if (d == NULL)
		pc_unresolved("opendir(dd) failed: %s", pc_errname(errno));
	while ((e = readdir(d)) != NULL) {
		if (n >= max)
			pc_unresolved("too many entries");
		snprintf(names[n++], 300, "%s", e->d_name);
	}
	(void)closedir(d);
	return n;
}

/* Add the posix_dent structures in buf[0..n) to ents; checks their layout. */
static inline void parse(const char *buf, long n)
{
	long off = 0;

	while (off < n) {
		const struct posix_dent *d = (const void *)(buf + off);
		size_t len, room;

		pc_check(off + (long)offsetof(struct posix_dent, d_name) < n, "an entry is cut off at byte %ld of %ld", off, n);
		pc_check(d->d_reclen % _Alignof(struct posix_dent) == 0, "d_reclen %lu breaks the alignment", (unsigned long)d->d_reclen);
		room = (size_t)(n - off) < (size_t)d->d_reclen ? (size_t)(n - off) : (size_t)d->d_reclen;
		pc_check(room > offsetof(struct posix_dent, d_name), "d_reclen %lu is too small", (unsigned long)d->d_reclen);
		len = strnlen(d->d_name, room - offsetof(struct posix_dent, d_name));
		pc_check(offsetof(struct posix_dent, d_name) + len + 1 <= (size_t)d->d_reclen, "d_reclen %lu cannot hold the name", (unsigned long)d->d_reclen);
		pc_check(len > 0, "an entry has an empty name");
		pc_check(len < sizeof ents[0].name, "an entry name is too long");
		pc_check(nents < MAXE, "too many entries");
		memcpy(ents[nents].name, d->d_name, len);
		ents[nents].name[len] = '\0';
		ents[nents].ino = d->d_ino;
		ents[nents].type = d->d_type;
		nents++;
		pc_check(d->d_reclen > 0, "d_reclen is 0");
		off += (long)d->d_reclen;
	}
	pc_check(off == n, "the entries add up to %ld bytes, the call returned %ld", off, n);
}

/* Read to the end with the given buffer size; every call must return bytes or 0 at the end. */
static inline void read_all(int fd, size_t bufsz)
{
	char *buf = malloc(bufsz);
	int calls = 0;

	if (buf == NULL)
		pc_unresolved("malloc failed");
	nents = 0;
	for (;;) {
		ssize_t r = posix_getdents(fd, buf, bufsz, 0);

		pc_check(r >= 0, "posix_getdents failed: %s", pc_errname(errno));
		if (r == 0)
			break;
		parse(buf, (long)r);
		pc_check(++calls < 1000, "posix_getdents never reached the end");
	}
	free(buf);
}

static inline int find(const char *name)
{
	int i;

	for (i = 0; i < nents; i++)
		if (strcmp(ents[i].name, name) == 0)
			return i;
	return -1;
}

/* ents must hold exactly the names in want[0..nw), each once. */
static inline void same_names(char want[][300], int nw)
{
	int i, j;

	pc_check(nents == nw, "posix_getdents gave %d entries, readdir gave %d", nents, nw);
	for (i = 0; i < nw; i++) {
		int c = 0;

		for (j = 0; j < nents; j++)
			if (strcmp(ents[j].name, want[i]) == 0)
				c++;
		pc_check(c == 1, "name \"%s\" appears %d times in the posix_getdents entries", want[i], c);
	}
}

int main(void)
{
	pc_begin("XSH.posix_getdents.003");

	int fd = make_dd();
	char want[MAXE][300];
	int nw = readdir_names(want, MAXE);
	long max = pathconf(".", _PC_NAME_MAX);

	pc_test();
	read_all(fd, sizeof(struct posix_dent) + (size_t)max + 1);
	same_names(want, nw);
	pc_pass();
}
