/*
 * utimes() sets the times; a null argument sets the current time.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.futimens.018
 * MUTANT    utimes.noop
 * MUTANT    utimes.swaps
 * MUTANT    utimes.null_ignored
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

int main(void)
{
	pc_begin("XSH.futimens.018");
	{
		struct timeval tv[2];
		struct timespec before, after, wa, wm;
		struct stat st;
		long long da, dm;
		int r;

		pc_make_file("f", "x");
		tv[0].tv_sec = 1000000000;
		tv[0].tv_usec = 250000;
		tv[1].tv_sec = 1100000000;
		tv[1].tv_usec = 750000;
		wa = ts(1000000000, 250000000);
		wm = ts(1100000000, 750000000);
		pc_test();
		r = utimes("f", tv);
		pc_check(r == 0, "utimes() returned %d (%s)", r, pc_errname(errno));
		st = lst("f");
		da = ns_diff(st.st_atim, wa);
		dm = ns_diff(st.st_mtim, wm);
		pc_check(da > -2000000000LL && da <= 1000000000LL, "st_atim is %lld.%09ld", (long long)st.st_atim.tv_sec,
			 st.st_atim.tv_nsec);
		pc_check(dm > -2000000000LL && dm <= 1000000000LL, "st_mtim is %lld.%09ld", (long long)st.st_mtim.tv_sec,
			 st.st_mtim.tv_nsec);
		old_times("f", 0);
		before = now();
		pc_check(utimes("f", NULL) == 0, "utimes(NULL) failed: %s", pc_errname(errno));
		after = now();
		after.tv_sec += 1;
		st = lst("f");
		pc_check(set_now(st.st_atim, before, after) && set_now(st.st_mtim, before, after),
			 "utimes(NULL): st_atim %lld, st_mtim %lld", (long long)st.st_atim.tv_sec, (long long)st.st_mtim.tv_sec);
	}
	pc_pass();
}
