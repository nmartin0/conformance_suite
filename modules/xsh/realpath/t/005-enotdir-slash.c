/*
 * realpath() of a non-directory with a trailing slash gives ENOTDIR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.realpath.005
 * MUTANT    realpath.slash_ignored
 * VARIANT   realpath.declared
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

#ifdef PC_PATH_MAX_CONST
static char rbuf[PATH_MAX];
#define RBUF rbuf
#else
#define RBUF NULL	/* without a constant {PATH_MAX}, only a null pointer is defined */
#endif

static struct stat st_of(const char *path)
{
	struct stat st;

	pc_check(stat(path, &st) == 0, "stat(%s) failed: %s", path, pc_errname(errno));
	return st;
}

/*
 * res, from realpath(what), must be an absolute pathname without dot,
 * dot-dot or symbolic link components, naming the entry base in the
 * directory dir.
 */
static void entry(const char *res, const char *what, const char *dir, const char *base)
{
	static char pre[PATH_MAX];
	struct stat st, d = st_of(dir);
	size_t i = 0, start, last = 0;

	pc_check(res != NULL, "realpath(\"%s\") failed: %s", what, pc_errname(errno));
	pc_check(res[0] == '/', "realpath(\"%s\") gave \"%s\", not an absolute pathname", what, res);
	pc_check(strlen(res) < sizeof pre, "realpath(\"%s\") gave a name too long to check", what);
	for (;;) {
		while (res[i] == '/')
			i++;
		start = i;
		while (res[i] != '\0' && res[i] != '/')
			i++;
		if (i == start)
			break;
		last = start;
		pc_check(!(i - start == 1 && res[start] == '.'), "realpath(\"%s\") gave \"%s\", with a dot", what, res);
		pc_check(!(i - start == 2 && res[start] == '.' && res[start + 1] == '.'),
			 "realpath(\"%s\") gave \"%s\", with a dot-dot", what, res);
		memcpy(pre, res, i);
		pre[i] = '\0';
		pc_check(lstat(pre, &st) == 0, "realpath(\"%s\") gave \"%s\", whose prefix \"%s\" does not exist", what,
			 res, pre);
		pc_check(!S_ISLNK(st.st_mode), "realpath(\"%s\") gave \"%s\", whose prefix \"%s\" is a symbolic link", what,
			 res, pre);
	}
	if (base == NULL) {
		pc_check(stat(res, &st) == 0 && same_file(&st, &d), "realpath(\"%s\") gave \"%s\", not a name of %s", what,
			 res, dir);
		return;
	}
	pc_check(strcmp(res + last, base) == 0, "realpath(\"%s\") gave \"%s\", not an entry named %s", what, res, base);
	memcpy(pre, res, last);
	pre[last > 1 ? last - 1 : 1] = '\0';
	pc_check(stat(pre, &st) == 0 && same_file(&st, &d), "realpath(\"%s\") gave \"%s\", whose directory is not %s",
		 what, res, dir);
}

/* realpath(name) fails with e. */
static void fails(const char *name, int e)
{
	char *r;
	int got;

	errno = 0;
	r = realpath(name, RBUF);
	got = errno;
	pc_check(r == NULL, "realpath(\"%s\") gave \"%s\", expected a null pointer with %s", name, r ? r : "", pc_errname(e));
	pc_check(got == e, "realpath(\"%s\"): errno %s, expected %s", name, pc_errname(got), pc_errname(e));
}

int main(void)
{
	pc_begin("XSH.realpath.005");
	{
		pc_make_file("f", "x");
		pc_make_symlink("f", "lf");
		pc_test();
		fails("f/", ENOTDIR);
		fails("lf/", ENOTDIR);
	}
	pc_pass();
}
