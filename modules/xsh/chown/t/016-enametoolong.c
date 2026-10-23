/*
 * A component longer than NAME_MAX gives ENAMETOOLONG.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.chown.016
 * MUTANT    chown.long_as_enoent
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

static int exists(const char *path)
{
	struct stat st;

	return lstat(path, &st) == 0;
}

static struct stat lst(const char *path)
{
	struct stat st;

	pc_check(lstat(path, &st) == 0, "lstat(%s) failed: %s", path, pc_errname(errno));
	return st;
}

static int dir_fd(const char *path)
{
	int fd = open(path, O_RDONLY | O_DIRECTORY);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	return fd;
}

static void long_name(char *buf, size_t size)
{
	long max = pathconf(".", _PC_NAME_MAX);

	if (max < 0 || max + 2 > (long)size)
		pc_unresolved("{NAME_MAX} is indeterminate or too large here (%ld)", max);
	memset(buf, 'n', (size_t)max + 1);
	buf[max + 1] = '\0';
}

/* 0: failed with one of the expected errors; 1: succeeded; 10 + errno: another errno. */
static int verdict(int r, int e1, int e2)
{
	if (r == 0)
		return 1;
	return errno == e1 || errno == e2 ? 0 : 10 + errno % 80;
}

/*
 * A group the unprivileged child is not in, and not g0: not its effective
 * group (65534) nor one of the supplementary groups it keeps from root.
 */
static gid_t foreign_gid(gid_t g0)
{
	gid_t g[256], c;
	int n = getgroups(256, g), i;

	for (c = 65533; c > 1; c--) {
		for (i = 0; i < n && g[i] != c; i++)
			;
		if (i == n && c != 65534 && c != getegid() && c != g0)
			return c;
	}
	pc_unresolved("no group outside the child's groups was found");
	return 0;
}

/* As root, before the child runs: c/own owned by the child, group g, mode m. */
static void prepare_own(gid_t g, mode_t m)
{
	struct stat st;

	pc_make_dir("c");
	pc_check(chmod("c", 0777) == 0, "chmod failed");
	pc_make_file("c/own", "x");
	pc_check(chown("c/own", 65534, g) == 0 && chmod("c/own", m) == 0, "chown or chmod failed: %s", pc_errname(errno));
	pc_check(stat("c/own", &st) == 0, "stat failed");
	if (st.st_uid != 65534 || st.st_gid != g || (st.st_mode & 07777) != m)
		pc_unresolved("the file system did not take owner 65534, group %ld and mode %#o", (long)g, (unsigned)m);
}

static void need_root(void)
{
	if (geteuid() != 0)
		pc_unresolved("needs appropriate privileges to give files away");
}

/* In the child: a regular file of its own, c/own (c is writable by all). */
static int make_own(mode_t mode)
{
	int fd = open("c/own", O_WRONLY | O_CREAT | O_EXCL, mode);

	if (fd < 0)
		return -1;
	return close(fd);
}

int main(void)
{
	pc_begin("XSH.chown.016");
	{
		int r = 0;
		char name[4096];

		long_name(name, sizeof name);
		pc_test();
		pc_expect_err(chown(name, (uid_t)-1, (gid_t)-1), ENAMETOOLONG, "chown(name, (uid_t)-1, (gid_t)-1)");
		(void)r;
	}
	pc_pass();
}
