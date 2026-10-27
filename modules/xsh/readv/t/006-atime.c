/*
 * readv() marks the access time.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.readv.006
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/uio.h>

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* 1 if a is later than b. */
static int ts_later(struct timespec a, struct timespec b)
{
	if (a.tv_sec != b.tv_sec)
		return a.tv_sec > b.tv_sec;
	return a.tv_nsec > b.tv_nsec;
}

int main(void)
{
	struct stat before, after;
	char a[3];
	struct iovec iov[1];
	int fd;

	pc_begin("XSH.readv.006");
	pc_make_file("f", "abcdef");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(fstat(fd, &before) == 0, "fstat failed");
	iov[0].iov_base = a; iov[0].iov_len = sizeof a;
	sleep_ms(2100);
	pc_test();
	pc_check(readv(fd, iov, 1) == 3, "readv() did not return 3");
	pc_check(fstat(fd, &after) == 0, "fstat failed after readv()");
	pc_check(ts_later(after.st_atim, before.st_atim), "the access time was not updated");
	pc_pass();
}
