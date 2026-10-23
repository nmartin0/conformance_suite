/*
 * utimensat() through an O_SEARCH descriptor skips the search check.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.futimens.025
 * MUTANT    utimensat.search_checked
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
 * Whether this file system keeps timestamps finer than a microsecond, as
 * seen from a new file's st_mtim (setup; a nanosecond clock gives a
 * multiple of 1000 only rarely, which merely loosens the check).
 */
static int fine_ns = -1;

static void probe_resolution(void)
{
	struct stat st;

	pc_make_file(".pc-res", "");
	pc_check(lstat(".pc-res", &st) == 0, "lstat failed: %s", pc_errname(errno));
	fine_ns = st.st_mtim.tv_nsec % 1000 != 0;
}

/*
 * The timestamp was set from want: not later than want and less than 1 s
 * earlier (XBD 4.12: resolution no coarser than a second); within 100 ns
 * where the file system keeps sub-microsecond times.
 */
static int set_from(struct timespec got, struct timespec want)
{
	long long d = ns_diff(want, got);

	if (fine_ns == -1)
		probe_resolution();
	return d >= 0 && d < (fine_ns ? 100 : 1000000000LL);
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
	return ns_diff(after, got) >= 0 && ns_diff(got, before) > -1000000000LL;
}

/*
 * Setup: the file system's clock is not ahead of this process's (as with a
 * remote file system's server), else times set to "now" look late.
 */
static void clock_agrees(void)
{
	struct stat st;

	pc_make_file(".pc-clock", "");
	pc_check(lstat(".pc-clock", &st) == 0, "lstat failed: %s", pc_errname(errno));
	if (ns_diff(st.st_mtim, now()) > 0)
		pc_unresolved("the file system's clock is ahead of the process's clock");
}

/* 0: failed with the expected error; 1: succeeded; 10 + errno: another errno. */
static int verdict(int r, int e)
{
	if (r == 0)
		return 1;
	return errno == e ? 0 : 10 + (errno < 86 ? errno : 86);
}

#define OLD_A 1000000000
#define OLD_M 1100000000

static int kept_old(const char *path)
{
	struct stat st;

	return lstat(path, &st) == 0 && st.st_atim.tv_sec == OLD_A && st.st_atim.tv_nsec == 0 && st.st_mtim.tv_sec == OLD_M &&
	       st.st_mtim.tv_nsec == 0;
}

/*
 * Setup only: give path the old, known times; a file system that cannot
 * keep them exactly makes the test UNRESOLVED.
 */
static void old_times(const char *path)
{
	struct timespec v[2];

	v[0] = ts(OLD_A, 0);
	v[1] = ts(OLD_M, 0);
	pc_check(utimensat(AT_FDCWD, path, v, 0) == 0, "setting old times on %s failed: %s", path, pc_errname(errno));
	if (!kept_old(path))
		pc_unresolved("the file system does not keep the times 1000000000 and 1100000000 exactly");
}

static int child(void)
{
#ifndef O_SEARCH
	return 96;
#else
	struct timespec v[2];
	int fd, dfd;

	if (mkdir("c/s", 0700) != 0 || (fd = open("c/s/f", O_WRONLY | O_CREAT | O_EXCL, 0644)) < 0 || close(fd) != 0)
		return 3;
	if ((dfd = open("c/s", O_SEARCH | O_DIRECTORY)) < 0 || chmod("c/s", 0600) != 0)
		return 3;
	v[0] = ts(1200000000, 0);
	v[1] = ts(1200000000, 0);
	return utimensat(dfd, "f", v, 0) == 0 ? 0 : 10;
#endif
}

int main(void)
{
	pc_begin("XSH.futimens.025");
	pc_searchable();
	{
		int r;
		pc_make_dir("c");
		pc_check(chmod("c", 0777) == 0, "chmod failed");
		pc_test();
		r = unprivileged(child);
		if (r == 96)
			pc_unresolved("<fcntl.h> does not define O_SEARCH");
		pc_check(r != 3, "the child could not prepare its file");
		pc_check(r == 0, "utimensat() through an O_SEARCH descriptor: check %d went wrong", r);
	}
	pc_pass();
}
