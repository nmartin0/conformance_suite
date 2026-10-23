/*
 * write() updates mtime and ctime.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.020
 * MUTANT    write.no_mtime
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
	struct stat before, after;
	int fd;

	pc_begin("XSH.write.020");
	pc_make_file("f", "0123");
	fd = open("f", O_WRONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(fstat(fd, &before) == 0, "fstat failed");
	sleep(2);
	pc_test();
	pc_check(write(fd, "ab", 2) == 2, "write failed");
	pc_check(fstat(fd, &after) == 0, "fstat failed");
	pc_check(after.st_mtim.tv_sec > before.st_mtim.tv_sec ||
	    (after.st_mtim.tv_sec == before.st_mtim.tv_sec && after.st_mtim.tv_nsec > before.st_mtim.tv_nsec),
	    "mtime not updated");
	pc_check(after.st_ctim.tv_sec > before.st_ctim.tv_sec ||
	    (after.st_ctim.tv_sec == before.st_ctim.tv_sec && after.st_ctim.tv_nsec > before.st_ctim.tv_nsec),
	    "ctime not updated");
	pc_pass();
}
