/*
 * chmod() by a non-member does not leave S_ISGID set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.chmod.007
 * VARIANT   chmod.setid_ignored
 * VARIANT   chmod.setgid_eperm
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

static unsigned bits(const char *path)
{
	struct stat st;

	pc_check(stat(path, &st) == 0, "stat(%s) failed: %s", path, pc_errname(errno));
	return (unsigned)(st.st_mode & 07777);
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

static gid_t other;

static int child(void)
{
	gid_t g[256];
	int n = getgroups(256, g), i;

	for (i = 0; i < n; i++)
		if (g[i] == other)
			return 3;
	if (getegid() == other)
		return 3;
	if (chmod("f", 02755) != 0)
		return errno == EPERM ? 4 : 10 + errno % 80;
	return 0;
}

int main(void)
{
	pc_begin("XSH.chmod.007");
	pc_searchable();
	{
		struct stat st;
		gid_t g[256];
		int n, i, r;

		if (geteuid() != 0)
			pc_unresolved("needs root to give a file an owner and a foreign group");
		/* a group the child is not in: it keeps root's supplementary groups */
		n = getgroups(256, g);
		for (other = 2; other < 1000; other++) {
			for (i = 0; i < n && g[i] != other; i++)
				;
			if (i == n && other != getegid() && other != 65534)
				break;
		}
		pc_make_file("f", "x");
		pc_check(chmod("f", 0755) == 0, "chmod failed");
		pc_check(chown("f", 65534, other) == 0, "chown failed: %s", pc_errname(errno));
		pc_check(stat("f", &st) == 0, "stat failed");
		if (st.st_uid != 65534 || st.st_gid != other)
			pc_unresolved("the file system did not take owner 65534 and group %ld", (long)other);
		pc_test();
		r = unprivileged(child);
		if (r == 3)
			pc_unresolved("group %ld is one of the child's groups after all", (long)other);
		pc_check(stat("f", &st) == 0, "stat failed");
		if (r == 4) {
			pc_check((st.st_mode & 07777) == 0755, "chmod() failed with EPERM but changed the mode to %#o",
				 (unsigned)(st.st_mode & 07777));
			pc_note("chmod() refused S_ISGID with EPERM");
		} else {
			pc_check(r == 0, "chmod() by the owner failed: %s", pc_errname(r >= 10 ? r - 10 : 0));
			pc_check(!(st.st_mode & S_ISGID), "S_ISGID was kept: the bits are %#o", (unsigned)(st.st_mode & 07777));
			pc_check((st.st_mode & 0777) == 0755, "the permission bits are %#o", (unsigned)(st.st_mode & 0777));
		}
	}
	pc_pass();
}
