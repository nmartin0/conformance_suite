/*
 * lstat() of anything but a symbolic link is stat().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fstatat.005
 * MUTANT    lstat.type_link
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
	pc_begin("XSH.fstatat.005");
	{
		struct stat a, b;
		const char *name[2] = { "f", "d" };
		int i;

		pc_make_file("f", "abc");
		pc_make_dir("d");
		for (i = 0; i < 2; i++) {
			struct stat ref = via_fd(name[i], O_RDONLY);
			pc_test();
			pc_check(lstat(name[i], &a) == 0, "lstat(%s) failed: %s", name[i], pc_errname(errno));
			pc_check(stat(name[i], &b) == 0, "stat(%s) failed: %s", name[i], pc_errname(errno));
			pc_check(a.st_mode == b.st_mode && a.st_size == b.st_size && same_file(&a, &b),
				 "lstat(%s) and stat(%s) disagree", name[i], name[i]);
			pc_check(same_file(&a, &ref), "lstat(%s) reports another file", name[i]);
		}
	}
	pc_pass();
}
