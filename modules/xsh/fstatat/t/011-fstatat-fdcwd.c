/*
 * fstatat(AT_FDCWD, ...) works from the working directory.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fstatat.011
 * MUTANT    fstatat.fdcwd_ebadf
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
	pc_begin("XSH.fstatat.011");
	{
		struct stat a, b, ref;

		pc_make_file("target", "abc");
		pc_make_symlink("target", "link");
		ref = via_fd("target", O_RDONLY);
		pc_test();
		pc_check(fstatat(AT_FDCWD, "link", &a, 0) == 0, "fstatat(AT_FDCWD) failed: %s", pc_errname(errno));
		pc_check(same_file(&a, &ref) && S_ISREG(a.st_mode), "fstatat(AT_FDCWD, link, 0) is not the target");
		pc_check(fstatat(AT_FDCWD, "link", &b, AT_SYMLINK_NOFOLLOW) == 0, "fstatat(AT_FDCWD, NOFOLLOW) failed: %s",
			 pc_errname(errno));
		pc_check(S_ISLNK(b.st_mode), "fstatat(AT_FDCWD, link, NOFOLLOW) is not the link");
	}
	pc_pass();
}
