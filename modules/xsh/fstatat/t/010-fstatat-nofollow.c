/*
 * AT_SYMLINK_NOFOLLOW makes fstatat() lstat().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fstatat.010
 * MUTANT    fstatat.ignores_nofollow
 * MUTANT    fstatat.always_nofollow
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

int main(void)
{
	pc_begin("XSH.fstatat.010");
	{
		struct stat a, b, ref;
		int dfd;

		pc_make_dir("d");
		pc_make_file("d/target", "x");
		pc_make_symlink("target", "d/link");
		ref = via_fd("d/target", O_RDONLY);
		dfd = open("d", O_RDONLY | O_DIRECTORY);
		pc_check(dfd >= 0, "open(d) failed: %s", pc_errname(errno));
		pc_test();
		pc_check(fstatat(dfd, "link", &a, AT_SYMLINK_NOFOLLOW) == 0, "fstatat(NOFOLLOW) failed: %s",
			 pc_errname(errno));
		pc_check(S_ISLNK(a.st_mode), "with AT_SYMLINK_NOFOLLOW st_mode %#o is not a link", (unsigned)a.st_mode);
		pc_check(!same_file(&a, &ref), "with AT_SYMLINK_NOFOLLOW the target was reported");
		pc_check(fstatat(dfd, "link", &b, 0) == 0, "fstatat(0) failed: %s", pc_errname(errno));
		pc_check(S_ISREG(b.st_mode) && same_file(&b, &ref), "without the flag the link was not followed");
	}
	pc_pass();
}
