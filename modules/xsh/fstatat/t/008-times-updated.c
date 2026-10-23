/*
 * stat() reports the times a write marked for update.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fstatat.008
 * MUTANT    stat.stale_times
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
	pc_begin("XSH.fstatat.008");
	{
		struct stat before, st;
		int fd, waited, moved = 0;

		pc_make_file("f", "");
		pc_check(stat("f", &before) == 0, "stat failed");
		fd = open("f", O_WRONLY);
		pc_check(fd >= 0, "open failed: %s", pc_errname(errno));
		pc_test();
		for (waited = 0; waited < 3000 && !moved; waited += 10) {
			sleep_ms(10);
			pc_check(write(fd, "x", 1) == 1, "write failed: %s", pc_errname(errno));
			pc_check(stat("f", &st) == 0, "stat failed: %s", pc_errname(errno));
			moved = ts_cmp(st.st_mtim, before.st_mtim) > 0;
		}
		pc_check(moved, "st_mtim did not move after writes for 3 s");
		pc_check(ts_cmp(st.st_ctim, before.st_ctim) > 0, "st_mtim moved but st_ctim did not");
	}
	pc_pass();
}
