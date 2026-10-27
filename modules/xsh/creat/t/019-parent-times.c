/*
 * Creating a file with creat() marks the parent directory's times.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creat.019
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

	pc_begin("XSH.creat.019");
	pc_make_dir("d");
	pc_check(stat("d", &before) == 0, "stat failed");
	sleep_ms(2100);
	pc_test();
	pc_expect_ok(creat("d/new", 0600), "creat(\"d/new\")");
	pc_check(stat("d", &after) == 0, "stat failed after creat()");
	pc_check(ts_later(after.st_mtim, before.st_mtim), "the parent's data modification time was not updated");
	pc_check(ts_later(after.st_ctim, before.st_ctim), "the parent's status change time was not updated");
	pc_pass();
}
