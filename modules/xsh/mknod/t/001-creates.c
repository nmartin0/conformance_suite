/*
 * mknod() creates a FIFO with the masked mode, owned by the caller.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mknod.001
 * MUTANT    mknod.noop
 * MUTANT    mknod.makes_file
 * MUTANT    mknod.returns_one
 * MUTANT    mknod.ignores_umask
 * MUTANT    mknod.ignores_mode
 * MUTANT    mknod.wrong_owner
 * MUTANT    mknod.wrong_group
 * VARIANT   mknod.parent_gid
 */
#include "pc.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
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

#ifdef PATH_MAX
#define PC_PATH_MAX_CONST 1	/* {PATH_MAX} is a constant in <limits.h> */
#else
#define PATH_MAX 4096	/* <limits.h> may omit it; tests only need a big buffer */
#endif

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

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
 * what fn returns (0 to 94); a child that cannot drop privileges, that
 * still has them (it can read the mode 0 file .pc-priv, as with a
 * capability), that reports 95 (a step not under test failed), or that
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
	if (WIFEXITED(st) && WEXITSTATUS(st) == 95)
		pc_unresolved("a step of the unprivileged child that is not under test failed");
	if (!WIFEXITED(st) || WEXITSTATUS(st) == 99)
		pc_unresolved("the unprivileged child could not run (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
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

/* A name one byte longer than {NAME_MAX} for the working directory. */
static void long_name(char *buf, size_t size)
{
	long max = pathconf(".", _PC_NAME_MAX);

	if (max < 0 || max + 2 > (long)size)
		pc_unresolved("{NAME_MAX} is indeterminate or too large here (%ld)", max);
	memset(buf, 'n', (size_t)max + 1);
	buf[max + 1] = '\0';
}

/*
 * For a child's exit status: 0 when r is -1 with errno e1 or e2, 1 when
 * the call succeeded, 10 + errno (at most 94) for another error.
 */
static int verdict(long r, int e1, int e2)
{
	if (r != -1)
		return 1;
	if (errno == e1 || errno == e2)
		return 0;
	return 10 + (errno > 0 && errno < 84 ? errno : 84);
}

/* What a verdict() means, for a failure note. */
static const char *verdict_text(int v)
{
	static char b[64];

	if (v == 1)
		return "succeeded";
	if (v >= 10 && v < 94) {
		snprintf(b, sizeof b, "failed with %s", pc_errname(v - 10));
		return b;
	}
	if (v == 94)
		return "failed with an errno above 83";
	snprintf(b, sizeof b, "gave child status %d", v);
	return b;
}

#define MK(p) mknod((p), S_IFIFO | 0644, 0)
#define MKAT(fd, p) mknodat((fd), (p), S_IFIFO | 0644, 0)
#define MKM(p, m) mknod((p), S_IFIFO | (m), 0)
#define CALL "mknod(..., S_IFIFO | 0644, 0)"
#define CALLAT "mknodat(..., S_IFIFO | 0644, 0)"
#define IS_NEW(m) S_ISFIFO(m)


int main(void)
{
	pc_begin("XSH.mknod.001");
	{
		struct stat st, g;
		int r;

		pc_make_dir("g");
		if (geteuid() == 0)
			pc_check(chown("g", (uid_t)-1, 4242) == 0, "chown(g) failed: %s", pc_errname(errno));
		g = lst("g");
		(void)umask(022);
		pc_test();
		r = MKM("g/p", 0640);
		pc_check(r == 0, "the call returned %d (%s)", r, pc_errname(errno));
		st = lst("g/p");
		pc_check(S_ISFIFO(st.st_mode), "g/p is not a FIFO (st_mode %#o)", (unsigned)st.st_mode);
		pc_check((st.st_mode & 0777) == 0640, "mode 0640 under umask 022 gave permission bits %#o", (unsigned)(st.st_mode & 0777));
		pc_check(st.st_uid == geteuid(), "st_uid %lu, effective user %lu", (unsigned long)st.st_uid,
			 (unsigned long)geteuid());
		pc_check(st.st_gid == getegid() || st.st_gid == g.st_gid,
			 "st_gid %lu is neither the effective group (%lu) nor the directory's (%lu)", (unsigned long)st.st_gid,
			 (unsigned long)getegid(), (unsigned long)g.st_gid);
		(void)umask(027);
		pc_check(MKM("g/q", 0777) == 0, "the call for g/q failed: %s", pc_errname(errno));
		st = lst("g/q");
		pc_check((st.st_mode & 0777) == 0750, "mode 0777 under umask 027 gave permission bits %#o", (unsigned)(st.st_mode & 0777));
	}
	pc_pass();
}
