/*
 * getcwd() works where the pathname is longer than {PATH_MAX}.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getcwd.008
 * MUTANT    getcwd.deep_enametoolong
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

/* The pathname p names the same directory as dir. */
static int names(const char *p, const struct stat *dir)
{
	struct stat st;

	return stat(p, &st) == 0 && same_file(&st, dir);
}

#define COMP 200

static char comp[COMP + 1];

/* Leave and remove the made levels, from the bottom (relative names only). */
static int unwind(int made, const char *start)
{
	int ok = 1;

	while (made-- > 0)
		if (chdir("..") != 0 || rmdir(comp) != 0)
			ok = 0;
	if (chdir(start) != 0)
		ok = 0;
	return ok;
}

int main(void)
{
	pc_begin("XSH.getcwd.008");
	{
		static char start[PATH_MAX];
		char *buf, *p;
		struct stat top, st;
		long pm = pathconf(".", _PC_PATH_MAX), want;
		size_t size, n, tail;
		int levels, made = 0, i, e, ok;
		char why[200] = "";

		pc_check(getcwd(start, sizeof start) == start, "getcwd() failed: %s", pc_errname(errno));
		top = st_of(".");
		memset(comp, 'd', COMP);
		want = pm > PATH_MAX ? pm : PATH_MAX;
		if (want > 65536)
			pc_notinuse("{PATH_MAX} is %ld, too large to exceed here", want);
		want += 300;
		levels = (int)((want - (long)strlen(start)) / (COMP + 1)) + 1;
		size = strlen(start) + (size_t)levels * (COMP + 1) + 64;
		buf = malloc(size);
		pc_check(buf != NULL, "malloc failed");
		pc_test();
		for (i = 0; i < levels; i++) {
			if (mkdir(comp, 0700) != 0)
				break;
			made++;
			if (chdir(comp) != 0)
				break;
		}
		if (i < levels) {
			e = errno;
			if (!unwind(made, start))
				pc_note("could not remove the deep tree");
			pc_unresolved("could not build the deep tree at level %d: %s", i, pc_errname(e));
		}
		errno = 0;
		p = getcwd(buf, size);
		e = errno;
		ok = p == buf;
		if (!ok)
			snprintf(why, sizeof why, "getcwd() at depth %ld bytes failed: %s",
				 (long)(strlen(start) + (size_t)levels * (COMP + 1)), pc_errname(e));
		if (ok) {
			n = strlen(buf);
			tail = (size_t)levels * (COMP + 1);
			ok = n > tail;
			for (i = 0; ok && i < levels; i++)
				ok = buf[n - tail + (size_t)i * (COMP + 1)] == '/' &&
				     memcmp(buf + n - tail + (size_t)i * (COMP + 1) + 1, comp, COMP) == 0;
			if (ok) {
				buf[n - tail] = '\0';
				ok = buf[0] == '/' && stat(buf, &st) == 0 && same_file(&st, &top);
			}
			if (!ok)
				snprintf(why, sizeof why, "getcwd() gave a %lu-byte name that does not name the directory",
					 (unsigned long)n);
		}
		if (!unwind(made, start))
			pc_unresolved("could not remove the deep tree");
		pc_check(ok, "%s", why);
	}
	pc_pass();
}
