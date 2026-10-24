/*
 * A write beyond the process's file size limit gives EFBIG.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fseek.013
 * MUTANT    fseek.wrong_errno
 * MUTANT    fseek.error_cleared
 * VARIANT   setvbuf.tiny_default
 * VARIANT   setvbuf.size_honored
 * VARIANT   setvbuf.own_unused
 * VARIANT   setvbuf.scribbles
 * VARIANT   fseek.lseek_first
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
 * (XBD 4.5). setgroups() is not POSIX, so it is used only where the system
 * is known to have it; elsewhere the child must already have none but its
 * own group, or the test is UNRESOLVED.
 */
#if defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || \
    defined(__DragonFly__) || defined(__APPLE__) || defined(__sun)
#if defined(__linux__)
extern int setgroups(size_t, const gid_t *);
#else
extern int setgroups(int, const gid_t *);
#endif
static void drop_groups(void)
{
	gid_t g = 65534;

	(void)setgroups(1, &g);
}
#else
static void drop_groups(void)
{
}
#endif

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

static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
}

/* A whence that is none of SEEK_SET, SEEK_CUR and SEEK_END. */
static int bad_whence(void)
{
	int w = 77;

	while (w == SEEK_SET || w == SEEK_CUR || w == SEEK_END)
		w++;
	return w;
}
#include <signal.h>
#include <sys/resource.h>

static int child(void)
{
	static char vb[4096];
	struct rlimit rl;
	sigset_t s;
	FILE *f;
	int i, r, e;

	sigemptyset(&s);
	sigaddset(&s, SIGXFSZ);
	if (sigprocmask(SIG_BLOCK, &s, NULL) != 0 || getrlimit(RLIMIT_FSIZE, &rl) != 0)
		return 99;
	rl.rlim_cur = 100;
	if (setrlimit(RLIMIT_FSIZE, &rl) != 0)
		return 98;
	f = fopen("big", "w");
	if (f == NULL || setvbuf(f, vb, _IOFBF, sizeof vb) != 0)
		return 97;
	for (i = 0; i < 200; i++)
		if (fputc('a', f) == EOF)
			return 96;
	errno = 0;
	r = fseek(f, 0, SEEK_SET);
	e = errno;
	if (r != -1)
		return 1;
	if (e != EFBIG)
		return 2;
	if (!ferror(f))
		return 3;
#if defined(_XOPEN_UNIX) && _XOPEN_UNIX != -1
	if (sigpending(&s) != 0 || !sigismember(&s, SIGXFSZ))
		return 4;
#endif
	return 0;
}

int main(void)
{
	pc_begin("XSH.fseek.013");
	{
		pid_t kid;
		int st;

		pc_test();
		kid = fork();
		NEED(kid != -1, "fork failed");
		if (kid == 0)
			_exit(child());
		NEED(waitpid(kid, &st, 0) == kid, "waitpid failed");
		NEED(WIFEXITED(st) && WEXITSTATUS(st) < 90, "the child could not run (status %#x)", (unsigned)st);
		pc_check(WEXITSTATUS(st) != 1, "fseek() succeeded beyond the file size limit");
		pc_check(WEXITSTATUS(st) != 2, "fseek() failed, but not with EFBIG");
		pc_check(WEXITSTATUS(st) != 3, "the error indicator is not set");
		pc_check(WEXITSTATUS(st) != 4, "SIGXFSZ is not pending");
		pc_check(WEXITSTATUS(st) == 0, "child status %d", WEXITSTATUS(st));
	}
	pc_pass();
}
