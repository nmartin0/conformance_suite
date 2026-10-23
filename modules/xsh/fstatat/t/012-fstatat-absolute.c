/*
 * fstatat() ignores fd for an absolute path.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fstatat.012
 * MUTANT    fstatat.ignores_fd_always
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
	pc_begin("XSH.fstatat.012");
	{
		char cwd[PATH_MAX], path[PATH_MAX + 8];
		struct stat st, ref;
		int dfd;

		pc_make_dir("d");
		pc_make_file("f", "outside");
		pc_make_file("d/f", "in");
		pc_check(getcwd(cwd, sizeof cwd) != NULL, "getcwd failed");
		snprintf(path, sizeof path, "%s/f", cwd);
		ref = via_fd("f", O_RDONLY);
		dfd = open("d", O_RDONLY | O_DIRECTORY);
		pc_check(dfd >= 0, "open(d) failed: %s", pc_errname(errno));
		pc_test();
		pc_check(fstatat(dfd, path, &st, 0) == 0, "fstatat(dfd, absolute) failed: %s", pc_errname(errno));
		pc_check(same_file(&st, &ref), "fstatat() resolved the absolute path from fd");
	}
	pc_pass();
}
