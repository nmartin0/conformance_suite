/*
 * access() checks each requested permission against the owner bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.access.001
 * MUTANT    access.noop
 * MUTANT    access.ignores_bits
 * MUTANT    access.any_bit
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

/* 0: failed with one of the expected errors; 1: succeeded; 2: another errno. */
static int verdict(int r, int e1, int e2)
{
	if (r == 0)
		return 1;
	return errno == e1 || errno == e2 ? 0 : 2;
}

/*
 * The n-th group ID (n from 0) of 2 or more that is none of this process's
 * real, effective or supplementary group IDs, nor 65534 (the unprivileged
 * child's group). Children inherit the supplementary list, so they are not
 * in it either.
 */
static gid_t foreign_gid(int n)
{
	gid_t g[1024];
	int k = getgroups(1024, g), i;
	gid_t c;

	if (k < 0)
		pc_unresolved("getgroups failed: %s", pc_errname(errno));
	for (c = 2; c < 60000; c++) {
		if (c == 65534 || c == getgid() || c == getegid())
			continue;
		for (i = 0; i < k && g[i] != c; i++)
			;
		if (i == k && n-- == 0)
			return c;
	}
	pc_unresolved("no free group ID found");
	return 0;
}

/* A file of the test's own with exactly these permission bits. */
static void file_mode(const char *path, mode_t mode)
{
	pc_make_file(path, "x");
	pc_check(chmod(path, mode) == 0, "chmod(%s) failed: %s", path, pc_errname(errno));
}

/* In the child: check number n gives the expected result, else return n. */
#define EXPECT_OK(n, call) do { if ((call) != 0) return (n); } while (0)
#define EXPECT_EACCES(n, call) do { if ((call) != -1 || errno != EACCES) return (n); } while (0)

static int child(void)
{
	int fd = open("c/f", O_WRONLY | O_CREAT | O_EXCL, 0600);

	if (fd < 0 || close(fd) != 0 || chmod("c/f", 0400) != 0)
		return 3;
	EXPECT_OK(10, access("c/f", R_OK));
	EXPECT_EACCES(11, access("c/f", W_OK));
	EXPECT_EACCES(12, access("c/f", X_OK));
	EXPECT_EACCES(13, access("c/f", R_OK | W_OK));
	EXPECT_OK(14, access("c/f", F_OK));
	if (chmod("c/f", 0700) != 0)
		return 3;
	EXPECT_OK(15, access("c/f", R_OK | W_OK | X_OK));
	EXPECT_OK(16, access("c/f", F_OK));
	if (chmod("c/f", 0444) != 0 || symlink("f", "c/l") != 0)
		return 3;
	EXPECT_OK(17, access("c/l", R_OK));
	EXPECT_EACCES(18, access("c/l", W_OK));
	if (chmod("c/f", 0) != 0)
		return 3;
	EXPECT_OK(19, access("c/f", F_OK));
	return 0;
}

int main(void)
{
	pc_begin("XSH.access.001");
	pc_searchable();
	{
		int r;
		pc_make_dir("c");
		pc_check(chmod("c", 0777) == 0, "chmod failed");
		pc_test();
		r = unprivileged(child);
		pc_check(r != 3, "the child could not prepare its file");
		pc_check(r == 0, "an access() check of the owner bits (check %d of the child went wrong)", r);
	}
	pc_pass();
}
