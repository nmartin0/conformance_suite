/*
 * With {STREAM_MAX} streams open, fdopen() gives EMFILE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fdopen.009
 * MUTANT    fdopen.wrong_errno
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
		if (geteuid() == 0 && (setgid(65534) != 0 || setuid(65534) != 0))
			_exit(99);
		if (geteuid() == 0)
			_exit(99);
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
#include <sys/resource.h>

/* Opens streams until {STREAM_MAX} are open (stdin, stdout and stderr count). */
static long fill_streams(void)
{
	struct rlimit rl;
	long max, i;

	errno = 0;
	max = sysconf(_SC_STREAM_MAX);
	if (max == -1 && errno == 0)
		pc_notinuse("{STREAM_MAX} is indeterminate");
	pc_check(max >= 8, "sysconf(_SC_STREAM_MAX) gave %ld", max);
	if (max > 4096)
		pc_notinuse("{STREAM_MAX} is %ld, too many to open", max);
	/* Descriptors must not run out first. */
	pc_check(getrlimit(RLIMIT_NOFILE, &rl) == 0, "getrlimit failed");
	if (rl.rlim_cur != RLIM_INFINITY && rl.rlim_cur < (rlim_t)max + 16) {
		if (rl.rlim_max != RLIM_INFINITY && rl.rlim_max < (rlim_t)max + 16)
			pc_unresolved("RLIMIT_NOFILE (%ld) is below {STREAM_MAX} (%ld)", (long)rl.rlim_max, max);
		rl.rlim_cur = (rlim_t)max + 16;
		pc_check(setrlimit(RLIMIT_NOFILE, &rl) == 0, "setrlimit failed");
	}
	for (i = 3; i < max; i++) {
		errno = 0;
		if (fopen("s", "r") == NULL)
			pc_unresolved("only %ld streams could be opened, below {STREAM_MAX} (%ld): %s", i, max, pc_errname(errno));
	}
	return max;
}

int main(void)
{
	pc_begin("XSH.fdopen.009");
	{
		FILE *f;
		int fd, e;

		pc_make_file("s", "x");
		(void)fill_streams();	/* before the test phase: no planted fault applies to it */
		pc_test();
		fd = open("s", O_RDONLY);
		NEED(fd >= 0, "open failed: %s", pc_errname(errno));
		errno = 0;
		f = fdopen(fd, "r");
		e = errno;
		pc_check(f == NULL, "fdopen() succeeded with {STREAM_MAX} streams open");
		pc_check(e == EMFILE, "fdopen() failed with %s, expected EMFILE", pc_errname(e));
	}
	pc_pass();
}
