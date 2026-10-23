/*
 * rename() without write or search permission gives EACCES.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.rename.025
 * MUTANT    rename.eacces_as_eperm
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

	if (geteuid() == 0 && chmod(".", 0711) != 0)
		pc_unresolved("chmod(.) failed: %s", pc_errname(errno));
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

/* 0: failed with one of the expected errors; 1: succeeded; 2: another errno. */
static int verdict(int r, int e1, int e2)
{
	if (r == 0)
		return 1;
	return errno == e1 || errno == e2 ? 0 : 2;
}

/* In the child: a file of its own, c/own (c is writable by all). */
static int make_own(void)
{
	int fd = open("c/own", O_WRONLY | O_CREAT | O_EXCL, 0600);

	if (fd < 0)
		return 99;
	return close(fd);
}

static int child(void)
{
	int v;

	if (make_own() != 0)
		return 99;
	if ((v = verdict(rename("c/own", "w/g"), EACCES, EACCES)) != 0)
		return v;
	if ((v = verdict(rename("w/f", "c/g"), EACCES, EACCES)) != 0)
		return v;
	return verdict(rename("s/f", "c/h"), EACCES, EACCES);
}

int main(void)
{
	pc_begin("XSH.rename.025");
	{
		pc_make_dir("c");
		pc_make_dir("w");
		pc_make_file("w/f", "x");
		pc_make_dir("s");
		pc_make_file("s/f", "x");
		pc_check(chmod("c", 0777) == 0 && chmod("w", 0555) == 0 && chmod("s", 0600) == 0, "chmod failed");
		pc_test();
		switch (unprivileged(child)) {
		case 0:
			break;
		case 1:
			pc_fail("a rename() needing write or search permission succeeded without permission");
		default:
			pc_fail("a rename() needing write or search permission failed with the wrong error");
		}
		pc_check(chmod("s", 0700) == 0, "chmod failed");
		pc_check(exists("c/own") && exists("w/f") && !exists("w/g") && !exists("c/g") && !exists("c/h"), "a failed rename() moved a file");
	}
	pc_pass();
}
