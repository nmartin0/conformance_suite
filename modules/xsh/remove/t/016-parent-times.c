/*
 * remove() marks the parent directory's times.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remove.016
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

	pc_begin("XSH.remove.016");
	pc_make_dir("p");
	pc_make_file("p/f", "x");
	pc_make_dir("p/d");
	pc_check(stat("p", &before) == 0, "stat failed");
	sleep_ms(2100);
	pc_test();
	pc_check(remove("p/f") == 0, "remove() of the file failed: %s", pc_errname(errno));
	pc_check(stat("p", &after) == 0, "stat failed");
	pc_check(ts_later(after.st_mtim, before.st_mtim), "removing the file did not update the parent's data modification time");
	pc_check(ts_later(after.st_ctim, before.st_ctim), "removing the file did not update the parent's status change time");
	before = after;
	sleep_ms(2100);
	pc_check(remove("p/d") == 0, "remove() of the directory failed: %s", pc_errname(errno));
	pc_check(stat("p", &after) == 0, "stat failed");
	pc_check(ts_later(after.st_mtim, before.st_mtim), "removing the directory did not update the parent's data modification time");
	pc_check(ts_later(after.st_ctim, before.st_ctim), "removing the directory did not update the parent's status change time");
	pc_pass();
}
