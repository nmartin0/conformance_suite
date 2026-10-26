/*
 * chdir() without search permission gives EACCES.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.chdir.007
 * MUTANT    chdir.eacces_as_eperm
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

static struct stat st_of(const char *path)
{
	struct stat st;

	pc_check(stat(path, &st) == 0, "stat(%s) failed: %s", path, pc_errname(errno));
	return st;
}

/* The working directory is still dir. */
static int still(const struct stat *dir)
{
	struct stat st;

	return stat(".", &st) == 0 && same_file(&st, dir);
}

/* chdir(path) fails with e and leaves the working directory as it was. */
static void fails(const char *path, int e)
{
	struct stat here = st_of(".");
	int r, got;

	errno = 0;
	r = chdir(path);
	got = errno;
	pc_check(r == -1, "chdir(\"%s\") returned %d, expected -1 with %s", path, r, pc_errname(e));
	pc_check(got == e, "chdir(\"%s\"): errno %s, expected %s", path, pc_errname(got), pc_errname(e));
	pc_check(still(&here), "chdir(\"%s\") failed but changed the working directory", path);
}

static int child(void)
{
	struct stat here, now;
	int v;

	if (stat(".", &here) != 0)
		return 95;
	if ((v = verdict(chdir("s/x"), EACCES, EACCES)) != 0)
		return v;
	if ((v = verdict(chdir("n"), EACCES, EACCES)) != 0)
		return v;
	if (stat(".", &now) != 0 || !same_file(&here, &now))
		return 2;
	return 0;
}

int main(void)
{
	pc_begin("XSH.chdir.007");
	pc_searchable();
	{
		int v;

		pc_make_dir("s");
		pc_make_dir("s/x");
		pc_make_dir("n");
		if (!(chmod("s/x", 0777) == 0 && chmod("s", 0600) == 0 && chmod("n", 0666) == 0))
			pc_unresolved("chmod failed");
		pc_test();
		v = unprivileged(child);
		pc_check(v != 2, "a failed chdir() changed the working directory");
		pc_check(v == 0, "chdir() needing search permission %s (EACCES expected)", verdict_text(v));
	}
	pc_pass();
}
