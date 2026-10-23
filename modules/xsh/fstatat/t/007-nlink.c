/*
 * st_nlink counts the file's hard links.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fstatat.007
 * MUTANT    stat.nlink_one
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
	pc_begin("XSH.fstatat.007");
	{
		struct stat a, b, c;

		pc_make_file("f", "x");
		pc_check(link("f", "g") == 0, "link failed: %s", pc_errname(errno));
		pc_test();
		pc_check(stat("f", &a) == 0 && stat("g", &b) == 0, "stat failed: %s", pc_errname(errno));
		pc_check(a.st_nlink == 2 && b.st_nlink == 2, "st_nlink %lu and %lu, the file has 2 links",
			 (unsigned long)a.st_nlink, (unsigned long)b.st_nlink);
		pc_check(unlink("g") == 0, "unlink failed: %s", pc_errname(errno));
		pc_check(stat("f", &c) == 0, "stat failed: %s", pc_errname(errno));
		pc_check(c.st_nlink == 1, "st_nlink %lu after unlink, 1 link left", (unsigned long)c.st_nlink);
	}
	pc_pass();
}
