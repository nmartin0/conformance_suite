/*
 * writev() marks the modification and status change times.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.writev.008
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
	struct iovec iov[1];
	int fd;

	pc_begin("XSH.writev.008");
	pc_make_file("f", "abc");
	fd = open("f", O_WRONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(fstat(fd, &before) == 0, "fstat failed");
	iov[0].iov_base = "X"; iov[0].iov_len = 1;
	sleep_ms(2100);
	pc_test();
	pc_check(writev(fd, iov, 1) == 1, "writev() did not return 1");
	pc_check(fstat(fd, &after) == 0, "fstat failed after writev()");
	pc_check(ts_later(after.st_mtim, before.st_mtim), "the data modification time was not updated");
	pc_check(ts_later(after.st_ctim, before.st_ctim), "the status change time was not updated");
	pc_pass();
}
