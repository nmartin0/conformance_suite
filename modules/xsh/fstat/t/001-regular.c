/*
 * fstat() describes an open regular file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fstat.001
 * MUTANT    fstat.size_off_by_one
 * MUTANT    fstat.type_dir
 * MUTANT    fstat.ino_wrong
 * MUTANT    fstat.nlink_zero
 * MUTANT    fstat.uid_wrong
 * MUTANT    fstat.returns_one
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
	pc_begin("XSH.fstat.001");
	{
		struct stat st, ref;
		int fd, r;

		fd = open("f", O_RDWR | O_CREAT | O_EXCL, 0604);
		pc_check(fd >= 0, "open failed: %s", pc_errname(errno));
		pc_check(write(fd, "1234567", 7) == 7, "write failed");
		pc_check(stat("f", &ref) == 0, "stat failed");
		pc_test();
		r = fstat(fd, &st);
		pc_check(r == 0, "fstat() returned %d (%s)", r, pc_errname(errno));
		pc_check(S_ISREG(st.st_mode), "st_mode %#o is not a regular file", (unsigned)st.st_mode);
		pc_check((st.st_mode & 07777) == 0604, "permission bits %#o, created 0604", (unsigned)(st.st_mode & 07777));
		pc_check(st.st_size == 7, "st_size %lld, 7 bytes written", (long long)st.st_size);
		pc_check(st.st_nlink == 1, "st_nlink %lu for a file with one link", (unsigned long)st.st_nlink);
		pc_check(st.st_uid == geteuid(), "st_uid %lu, effective user %lu", (unsigned long)st.st_uid,
			 (unsigned long)geteuid());
		pc_check(same_file(&st, &ref), "st_dev/st_ino differ from stat() of the name");
	}
	pc_pass();
}
