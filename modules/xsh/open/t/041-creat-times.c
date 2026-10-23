/*
 * Creating a file sets its times and updates the parent's.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.041
 * MUTANT    open.creat_old_times
 * MUTANT    open.creat_restores_parent_times
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
	struct stat before, after, f;
	int fd;

	pc_begin("XSH.open.041");
	pc_make_dir("d");
	pc_require_space();
	pc_check(stat("d", &before) == 0, "stat d failed");
	sleep(2);
	pc_test();
	fd = open("d/n", O_WRONLY | O_CREAT | O_EXCL, 0600);
	pc_expect_ok(fd, "open(\"d/n\", O_WRONLY|O_CREAT|O_EXCL)");
	pc_check(stat("d", &after) == 0, "stat d failed");
	pc_check(fstat(fd, &f) == 0, "fstat failed");
	pc_check(ts_after(after.st_mtim, before.st_mtim), "parent's mtime not updated");
	pc_check(ts_after(after.st_ctim, before.st_ctim), "parent's ctime not updated");
	pc_check(ts_after(f.st_atim, before.st_mtim), "new file's atime is old");
	pc_check(ts_after(f.st_mtim, before.st_mtim), "new file's mtime is old");
	pc_check(ts_after(f.st_ctim, before.st_ctim), "new file's ctime is old");
	pc_pass();
}
