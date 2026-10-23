/*
 * fstat() reports the type of a pipe, socket and directory.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fstat.004
 * MUTANT    fstat.type_reg
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
	pc_begin("XSH.fstat.004");
	{
		struct stat st;
		int p[2], s, d;

		pc_check(pipe(p) == 0, "pipe failed: %s", pc_errname(errno));
		s = socket(AF_UNIX, SOCK_STREAM, 0);
		pc_check(s >= 0, "socket failed: %s", pc_errname(errno));
		pc_make_dir("d");
		d = open("d", O_RDONLY | O_DIRECTORY);
		pc_check(d >= 0, "open(d) failed: %s", pc_errname(errno));
		pc_test();
		pc_check(fstat(p[0], &st) == 0 && S_ISFIFO(st.st_mode), "the read end of a pipe is not S_ISFIFO");
		pc_check(fstat(p[1], &st) == 0 && S_ISFIFO(st.st_mode), "the write end of a pipe is not S_ISFIFO");
		pc_check(fstat(s, &st) == 0 && S_ISSOCK(st.st_mode), "a socket is not S_ISSOCK");
		pc_check(fstat(d, &st) == 0 && S_ISDIR(st.st_mode), "a directory is not S_ISDIR");
	}
	pc_pass();
}
