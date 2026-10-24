/*
 * fdopendir() of a bad or non-directory descriptor fails.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.opendir.006
 * MUTANT    fdopendir.bad_as_enotdir
 * MUTANT    fdopendir.file_ok
 */
#include "pc.h"
#include <dirent.h>
#include <stddef.h>
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

/* 0: failed with the expected error; 1: succeeded; 10 + errno: another errno. */
static int verdict_p(const void *p, int e)
{
	if (p != NULL)
		return 1;
	return errno == e ? 0 : 10 + errno % 80;
}

#define MAXN 64

/* The names a stream returns from its current position to the end. */
struct names {
	int n, dots, dotdots, empty, err;
	char name[MAXN][256];
};

static void read_all(DIR *d, struct names *v)
{
	struct dirent *e;

	memset(v, 0, sizeof *v);
	for (;;) {
		errno = 0;
		e = readdir(d);
		if (e == NULL) {
			v->err = errno;
			return;
		}
		if (strcmp(e->d_name, ".") == 0)
			v->dots++;
		else if (strcmp(e->d_name, "..") == 0)
			v->dotdots++;
		else if (e->d_name[0] == '\0')
			v->empty++;
		else if (v->n < MAXN && strlen(e->d_name) < 256)
			strcpy(v->name[v->n++], e->d_name);
		else
			v->n = MAXN + 1;	/* too many: an error in itself here */
	}
}

/* How many times name is in v. */
static int count(const struct names *v, const char *name)
{
	int i, c = 0;

	for (i = 0; i < v->n && i < MAXN; i++)
		c += strcmp(v->name[i], name) == 0;
	return c;
}

/*
 * The entries are exactly the n names given (each once), with at most one
 * dot and one dot-dot, no empty name and no error; else a description of
 * what is wrong (static storage).
 */
static const char *exactly(const struct names *v, const char *const *want, int n)
{
	static char why[512];
	int i;

	if (v->err != 0)
		snprintf(why, sizeof why, "readdir() failed: %s", pc_errname(v->err));
	else if (v->empty != 0)
		snprintf(why, sizeof why, "%d entries with an empty name", v->empty);
	else if (v->dots > 1 || v->dotdots > 1)
		snprintf(why, sizeof why, "dot returned %d times, dot-dot %d times", v->dots, v->dotdots);
	else if (v->n != n)
		snprintf(why, sizeof why, "%d other entries, expected %d", v->n, n);
	else {
		for (i = 0; i < n; i++)
			if (count(v, want[i]) != 1) {
				snprintf(why, sizeof why, "\"%s\" returned %d times", want[i], count(v, want[i]));
				return why;
			}
		return NULL;
	}
	return why;
}

static DIR *open_dir(const char *path)
{
	DIR *d = opendir(path);

	pc_check(d != NULL, "opendir(%s) failed: %s", path, pc_errname(errno));
	return d;
}

/* Three plain files a, b and c in a new directory d. */
static void abc(void)
{
	pc_make_dir("d");
	pc_make_file("d/a", "x");
	pc_make_file("d/b", "x");
	pc_make_file("d/c", "x");
}

static const char *const ABC[] = { "a", "b", "c" };

int main(void)
{
	pc_begin("XSH.opendir.006");
	{
		void *p;
		struct dirent **nl;
		int fd, bad;

		pc_make_file("f", "x");
		fd = open("f", O_RDONLY);
		pc_check(fd >= 0, "open failed: %s", pc_errname(errno));
		bad = pc_lowest_free_fd();
		pc_test();
		errno = 0;
		p = fdopendir(bad);
		pc_check(p == NULL && errno == EBADF, "fdopendir(bad): returned %p, errno %s, expected EBADF", p, pc_errname(errno));
		errno = 0;
		p = fdopendir(fd);
		pc_check(p == NULL && errno == ENOTDIR, "fdopendir(fd): returned %p, errno %s, expected ENOTDIR", p, pc_errname(errno));
		(void)p;
		(void)nl;
	}
	pc_pass();
}
