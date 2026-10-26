/*
 * A stream on a terminal is not fully buffered.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fopen.019
 * MUTANT    fopen.full_tty
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static int same_file(const struct stat *a, const struct stat *b)
{
	return a->st_dev == b->st_dev && a->st_ino == b->st_ino;
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

static int ts_cmp(struct timespec a, struct timespec b)
{
	if (a.tv_sec != b.tv_sec)
		return a.tv_sec < b.tv_sec ? -1 : 1;
	if (a.tv_nsec != b.tv_nsec)
		return a.tv_nsec < b.tv_nsec ? -1 : 1;
	return 0;
}

#ifndef PATH_MAX
#define PATH_MAX 4096	/* <limits.h> may omit it; tests only need a big buffer */
#endif

/*
 * Setup for unprivileged(): when run as root, make the working directory
 * searchable for the child (relative paths need nothing above it). It is
 * called right after pc_begin(), in the setup phase, so no planted fault
 * applies to it.
 */
static void pc_searchable(void)
{
	if (geteuid() == 0 && chmod(".", 0711) != 0)
		pc_unresolved("chmod(.) failed: %s", pc_errname(errno));
	/* A file no one may read: the child can open it only with privileges. */
	pc_make_file(".pc-priv", "");
	if (chmod(".pc-priv", 0) != 0)
		pc_unresolved("chmod(.pc-priv) failed: %s", pc_errname(errno));
}

/*
 * Supplementary groups: a child that drops root keeps root's supplementary
 * groups, which would put it in the group class of files root created
 * (XBD 4.5). setgroups() is not POSIX: pc_drop_groups() uses it where the
 * system has it (found when the suite is built); either way the child must
 * then have no group but its own, or the test is UNRESOLVED.
 */
static void drop_groups(void)
{
	(void)pc_drop_groups();	/* core/pcgroups.c: setgroups() where the system has it */
}

/* After dropping privileges: no supplementary group but the child's own. */
static int groups_dropped(void)
{
	gid_t gs[64];
	int n = getgroups(64, gs), i;

	if (n < 0)
		return 0;
	for (i = 0; i < n; i++)
		if (gs[i] != getegid() && gs[i] != getgid())
			return 0;
	return 1;
}

/*
 * Run fn in a child that has no appropriate privileges: when the test runs
 * as root, the child takes user and group 65534 (the working directory is
 * made searchable for it; relative paths need nothing above it). Returns
 * what fn returns (0 to 96); a child that cannot drop privileges, that
 * still has them (it can read the mode 0 file .pc-priv, as with a
 * capability), or that dies, makes the test UNRESOLVED.
 */
static int unprivileged(int (*fn)(void))
{
	pid_t kid;
	int st;

	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		if (geteuid() == 0) {
			drop_groups();
			if (setgid(65534) != 0 || setuid(65534) != 0)
				_exit(99);
		}
		if (geteuid() == 0)
			_exit(99);
		if (!groups_dropped())
			_exit(98);
		{
			int pfd = open(".pc-priv", O_RDONLY);
			if (pfd >= 0)
				_exit(97);
		}
		_exit(fn());
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (WIFEXITED(st) && WEXITSTATUS(st) == 97)
		pc_unresolved("the child still has appropriate privileges (it can read a mode 0 file)");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 98)
		pc_unresolved("the unprivileged child kept supplementary groups (no way to drop them here)");
	if (!WIFEXITED(st) || WEXITSTATUS(st) == 99)
		pc_unresolved("the unprivileged child could not run (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

/* The status of path from a descriptor of it (setup: no mutants apply). */
static struct stat via_fd(const char *path, int flags)
{
	struct stat st;
	int fd = open(path, flags);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	pc_check(fstat(fd, &st) == 0, "fstat(%s) failed: %s", path, pc_errname(errno));
	(void)close(fd);
	return st;
}

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static long slurp(const char *path, char *buf, size_t size)
{
	int fd = open(path, O_RDONLY);
	long n = 0, r;

	NEED(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	while ((size_t)n < size - 1 && (r = read(fd, buf + n, size - 1 - (size_t)n)) > 0)
		n += r;
	(void)close(fd);
	buf[n] = '\0';
	return n;
}

/* The file holds exactly s. */
static int holds(const char *path, const char *s)
{
	char buf[256];

	return slurp(path, buf, sizeof buf) == (long)strlen(s) && strcmp(buf, s) == 0;
}

/* fopen(path, mode) must fail with err (or with err2, when it is not 0). */
static void fails(const char *path, const char *mode, int err, int err2)
{
	FILE *f;
	int e;

	errno = 0;
	f = fopen(path, mode);
	e = errno;
	if (f != NULL) {
		(void)fclose(f);
		pc_check(0, "fopen(\"%s\", \"%s\") succeeded", path, mode);
	}
	pc_check(e == err || (err2 != 0 && e == err2), "fopen(\"%s\", \"%s\") failed with %s, expected %s%s%s", path, mode,
		 pc_errname(e), pc_errname(err), err2 ? " or " : "", err2 ? pc_errname(err2) : "");
}

/* fopen() that must succeed. */
static FILE *opened(const char *path, const char *mode)
{
	FILE *f;

	errno = 0;
	f = fopen(path, mode);
	pc_check(f != NULL, "fopen(\"%s\", \"%s\") failed: %s", path, mode, pc_errname(errno));
	return f;
}

static int getfl(FILE *f)
{
	int fl = fcntl(fileno(f), F_GETFL);

	NEED(fl != -1, "fcntl(F_GETFL) failed: %s", pc_errname(errno));
	return fl;
}

static int cloexec(int fd)
{
	int fl = fcntl(fd, F_GETFD);

	NEED(fl != -1, "fcntl(F_GETFD) failed: %s", pc_errname(errno));
	return (fl & FD_CLOEXEC) != 0;
}
#include <poll.h>

int main(void)
{
	pc_begin("XSH.fopen.019");
	{
		struct pollfd p;
		char buf[16];
		FILE *f;
		int m;
		ssize_t n;

		m = posix_openpt(O_RDWR | O_NOCTTY);
		if (m == -1)
			pc_unresolved("posix_openpt failed: %s", pc_errname(errno));
		if (grantpt(m) != 0 || unlockpt(m) != 0 || ptsname(m) == NULL)
			pc_unresolved("the pseudo-terminal could not be set up: %s", pc_errname(errno));
		pc_test();
		f = opened(ptsname(m), "w");
		NEED(fputs("ab\n", f) >= 0, "fputs failed");
		p.fd = m;
		p.events = POLLIN;
		pc_check(poll(&p, 1, 2000) == 1, "nothing reached the master without a flush");
		n = read(m, buf, sizeof buf);
		pc_check(n >= 2 && buf[0] == 'a' && buf[1] == 'b', "the master read %ld bytes, not the line", (long)n);
	}
	pc_pass();
}
