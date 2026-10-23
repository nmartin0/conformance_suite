/*
 * lstat() describes the symbolic link itself.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fstatat.004
 * MUTANT    lstat.follows
 * MUTANT    lstat.size_with_nul
 * MUTANT    lstat.size_zero
 * MUTANT    lstat.nlink_zero
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
	pc_begin("XSH.fstatat.004");
	{
		struct stat st, ref;
		int r;

		pc_make_file("target", "x");
		pc_make_symlink("target", "link");	/* contents: 6 bytes */
		ref = via_fd("target", O_RDONLY);
		pc_test();
		r = lstat("link", &st);
		pc_check(r == 0, "lstat(link) failed: %s", pc_errname(errno));
		pc_check(S_ISLNK(st.st_mode), "st_mode %#o is not a symbolic link", (unsigned)st.st_mode);
		pc_check(st.st_size == 6, "st_size %lld, the link holds \"target\" (6 bytes)", (long long)st.st_size);
		pc_check(st.st_nlink == 1, "st_nlink %lu for a link with one name", (unsigned long)st.st_nlink);
		pc_check(st.st_uid == geteuid(), "st_uid %lu, effective user %lu", (unsigned long)st.st_uid,
			 (unsigned long)geteuid());
		pc_check(!same_file(&st, &ref), "lstat() reported the target, not the link");
	}
	pc_pass();
}
