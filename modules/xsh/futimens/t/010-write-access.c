/*
 * Write access suffices for the current time, not for given times.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.futimens.010
 * MUTANT    utimensat.perm_owner_only
 * MUTANT    utimensat.eperm_as_eacces
 * MUTANT    futimens.eperm_as_eacces
 * MUTANT    utimensat.any_now_ok
 * VARIANT   futimens.utime_now_m1
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
 * measured in setup by storing a time with tv_nsec 999999999 and reading
 * back how much of it the file system kept.
 */
static long long res_ns = -1;	/* how much a stored time may be truncated by */

static void probe_resolution(void)
{
	struct timespec t[2] = { { 1000000000, 999999999 }, { 1000000000, 999999999 } };
	struct stat st;

	pc_make_file(".pc-res", "");
	if (utimensat(AT_FDCWD, ".pc-res", t, 0) != 0 || lstat(".pc-res", &st) != 0 || st.st_mtim.tv_sec != 1000000000)
		res_ns = 1000000000LL;	/* cannot tell: allow what XBD 4.12 allows */
	else
		res_ns = 999999999LL - st.st_mtim.tv_nsec + 1;
	if (res_ns < 100)
		res_ns = 100;
}

/*
 * The timestamp was set from want: not later than want and less than 1 s
 * earlier (XBD 4.12: resolution no coarser than a second); within the
 * truncation measured in setup (at least 100 ns) where it is finer.
 */
static int set_from(struct timespec got, struct timespec want)
{
	long long d = ns_diff(want, got);

	if (res_ns == -1)
		probe_resolution();
	return d >= 0 && d < res_ns;
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
	static const long mix[3][2] = { { 0, 0 }, { UTIME_NOW, UTIME_OMIT }, { UTIME_NOW, 0 } };
	struct timespec v[2], m;
	struct stat st;
	int fd, x, i;

	if (utimensat(AT_FDCWD, "c/w", NULL, 0) != 0)
		return 10;
	v[0] = ts(0, UTIME_NOW);
	v[1] = ts(0, UTIME_NOW);
	if (utimensat(AT_FDCWD, "c/w", v, 0) != 0)
		return 11;
	if (stat("c/w", &st) != 0)
		return 3;
	m = st.st_mtim;
	for (i = 0; i < 3; i++) {
		v[0] = ts(1200000000, mix[i][0]);
		v[1] = ts(1200000000, mix[i][1]);
		if ((x = verdict(utimensat(AT_FDCWD, "c/w", v, 0), EPERM)) != 0)
			return x == 1 ? 1 : 12 + i;
		if (stat("c/w", &st) != 0 || ts_cmp(st.st_mtim, m) != 0)
			return 20 + i;
	}
	v[0] = ts(1200000000, 0);
	v[1] = ts(1200000000, 0);
	if ((fd = open("c/w", O_WRONLY)) < 0)
		return 3;
	if ((x = verdict(futimens(fd, v), EPERM)) != 0)
		return x == 1 ? 1 : 15;
	return 0;
}

int main(void)
{
	pc_begin("XSH.futimens.010");
	pc_searchable();
	{
		int r;
		if (geteuid() != 0)
			pc_unresolved("needs root to create files of another user");
		pc_make_dir("c");
		pc_make_file("c/w", "x");
		pc_make_file("c/r", "x");
		pc_check(chmod("c/w", 0666) == 0 && chmod("c/r", 0644) == 0, "chmod failed");
		if (lst("c/w").st_uid == 65534)
			pc_unresolved("files root creates here belong to user 65534 (a root-squashing file system)");
		old_times("c/w");
		old_times("c/r");
		pc_test();
		r = unprivileged(child);
		pc_check(r != 3, "the child could not prepare its file");
		pc_check(r != 1, "time changes on a writable file of another user: a call that should fail succeeded");
		pc_check(r == 0, "time changes on a writable file of another user: check %d went wrong", r);
	}
	pc_pass();
}
