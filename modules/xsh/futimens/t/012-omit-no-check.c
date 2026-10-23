/*
 * Both UTIME_OMIT makes no ownership or permission check.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.futimens.012
 * MUTANT    utimensat.omit_checked
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
#include <sys/time.h>
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
}

/*
 * Run fn in a child that has no appropriate privileges: when the test runs
 * as root, the child takes user and group 65534 (the working directory is
 * made searchable for it; relative paths need nothing above it). Returns
 * what fn returns (0 to 98); a child that cannot drop privileges, or that
 * dies, makes the test UNRESOLVED.
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
		_exit(fn());
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
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

static struct stat lst(const char *path)
{
	struct stat st;

	pc_check(lstat(path, &st) == 0, "lstat(%s) failed: %s", path, pc_errname(errno));
	return st;
}

static void long_name(char *buf, size_t size)
{
	long max = pathconf(".", _PC_NAME_MAX);

	if (max < 0 || max + 2 > (long)size)
		pc_unresolved("{NAME_MAX} is indeterminate or too large here (%ld)", max);
	memset(buf, 'n', (size_t)max + 1);
	buf[max + 1] = '\0';
}

static int dir_fd(const char *path)
{
	int fd = open(path, O_RDONLY | O_DIRECTORY);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	return fd;
}

static struct timespec ts(time_t s, long ns)
{
	struct timespec v;

	v.tv_sec = s;
	v.tv_nsec = ns;
	return v;
}

/* Nanoseconds from b to a (a - b), saturating far outside a few hours. */
static long long ns_diff(struct timespec a, struct timespec b)
{
	long long s = (long long)a.tv_sec - (long long)b.tv_sec;

	if (s > 100000)
		s = 100000;
	if (s < -100000)
		s = -100000;
	return s * 1000000000LL + (a.tv_nsec - b.tv_nsec);
}

/*
 * The timestamp was set from want: not later than want, and less than 2 s
 * earlier (the coarsest timestamp resolution of common file systems).
 */
static int set_from(struct timespec got, struct timespec want)
{
	long long d = ns_diff(want, got);

	return d >= 0 && d < 2000000000LL;
}

static struct timespec now(void)
{
	struct timespec v;

	pc_check(clock_gettime(CLOCK_REALTIME, &v) == 0, "clock_gettime failed: %s", pc_errname(errno));
	return v;
}

/* The timestamp was set to the current time, read between before and after. */
static int set_now(struct timespec got, struct timespec before, struct timespec after)
{
	return ns_diff(after, got) >= 0 && ns_diff(got, before) > -2000000000LL;
}

/* 0: failed with the expected error; 1: succeeded; 10 + errno: another errno. */
static int verdict(int r, int e)
{
	if (r == 0)
		return 1;
	return errno == e ? 0 : 10 + errno % 80;
}

#define OLD_A 1000000000
#define OLD_M 1100000000

/* Give path the old, known times (setup: no planted fault applies). */
static void old_times(const char *path, int flag)
{
	struct timespec v[2];

	v[0] = ts(OLD_A, 0);
	v[1] = ts(OLD_M, 0);
	pc_check(utimensat(AT_FDCWD, path, v, flag) == 0, "setting old times on %s failed: %s", path, pc_errname(errno));
}

static int kept_old(const char *path)
{
	struct stat st;

	return lstat(path, &st) == 0 && st.st_atim.tv_sec == OLD_A && st.st_atim.tv_nsec == 0 && st.st_mtim.tv_sec == OLD_M &&
	       st.st_mtim.tv_nsec == 0;
}

static int child(void)
{
	struct timespec v[2];

	v[0] = ts(1200000000, UTIME_OMIT);
	v[1] = ts(1200000000, UTIME_OMIT);
	if (utimensat(AT_FDCWD, "c/r", v, 0) != 0)
		return 10;
	return 0;
}

int main(void)
{
	pc_begin("XSH.futimens.012");
	pc_searchable();
	{
		int r;
		if (geteuid() != 0)
			pc_unresolved("needs root to create files of another user");
		pc_make_dir("c");
		pc_make_file("c/w", "x");
		pc_make_file("c/r", "x");
		pc_check(chmod("c/w", 0666) == 0 && chmod("c/r", 0644) == 0, "chmod failed");
		old_times("c/w", 0);
		old_times("c/r", 0);
		pc_test();
		r = unprivileged(child);
		pc_check(r != 3, "the child could not prepare its file");
		pc_check(r != 1, "both UTIME_OMIT on a file of another user: a call that should fail succeeded");
		pc_check(r == 0, "both UTIME_OMIT on a file of another user: check %d went wrong", r);
		pc_check(kept_old("c/r"), "both UTIME_OMIT changed the times");
	}
	pc_pass();
}
