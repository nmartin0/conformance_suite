/*
 * fstatat() resolves a relative path from fd.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fstatat.009
 * MUTANT    fstatat.ignores_fd
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
	pc_begin("XSH.fstatat.009");
	{
		struct stat st, ref;
		int dfd, r;

		pc_make_dir("d");
		pc_make_file("d/f", "inside");
		pc_make_file("f", "outside, longer");
		ref = via_fd("d/f", O_RDONLY);
		dfd = open("d", O_RDONLY | O_DIRECTORY);
		pc_check(dfd >= 0, "open(d) failed: %s", pc_errname(errno));
		pc_test();
		r = fstatat(dfd, "f", &st, 0);
		pc_check(r == 0, "fstatat() failed: %s", pc_errname(errno));
		pc_check(same_file(&st, &ref), "fstatat() reported a file other than d/f");
		pc_check(st.st_size == 6, "st_size %lld, d/f has 6 bytes", (long long)st.st_size);
	}
	pc_pass();
}
