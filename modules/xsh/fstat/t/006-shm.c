/*
 * fstat() of a shared memory object.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fstat.006
 * MUTANT    fstat.size_off_by_one
 * MUTANT    fstat.uid_wrong
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
	pc_begin("XSH.fstat.006");
	{
		char name[64];
		struct stat st;
		int fd;

		snprintf(name, sizeof name, "/pc-fstat-%ld", (long)getpid());
		(void)shm_unlink(name);
		fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0640);
		pc_check(fd >= 0, "shm_open failed: %s", pc_errname(errno));
		(void)shm_unlink(name);
		pc_check(ftruncate(fd, 4096) == 0, "ftruncate failed: %s", pc_errname(errno));
		pc_test();
		pc_check(fstat(fd, &st) == 0, "fstat() failed: %s", pc_errname(errno));
		pc_check(st.st_size == 4096, "st_size %lld, truncated to 4096", (long long)st.st_size);
		pc_check(st.st_uid == geteuid(), "st_uid %lu, effective user %lu", (unsigned long)st.st_uid,
			 (unsigned long)geteuid());
		pc_check((st.st_mode & 0666) == 0640, "read/write bits %#o, created 0640", (unsigned)(st.st_mode & 0666));
	}
	pc_pass();
}
