/*
 * O_TRUNC updates the file's mtime and ctime.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.042
 * MUTANT    open.trunc_ignored
 * MUTANT    open.trunc_keeps_times
 * VARIANT   open.accmode_bits
 * VARIANT   open.coarse_times
 */
#include "pc.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

/* a later than b */
static int ts_after(struct timespec a, struct timespec b)
{
	return a.tv_sec > b.tv_sec ||
	    (a.tv_sec == b.tv_sec && a.tv_nsec > b.tv_nsec);
}

int main(void)
{
	struct stat before, after;
	int fd;

	pc_begin("XSH.open.042");
	pc_make_file("f", "data");
	pc_check(stat("f", &before) == 0, "stat failed");
	sleep(2);
	pc_test();
	fd = open("f", O_WRONLY | O_TRUNC);
	pc_expect_ok(fd, "open(\"f\", O_WRONLY|O_TRUNC)");
	pc_check(fstat(fd, &after) == 0, "fstat failed");
	pc_check(after.st_size == 0, "file not truncated");
	pc_check(ts_after(after.st_mtim, before.st_mtim), "mtime not updated");
	pc_check(ts_after(after.st_ctim, before.st_ctim), "ctime not updated");
	pc_pass();
}
