/*
 * fsync() of a written regular file returns 0 and leaves it as it was.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fsync.001
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

int main(void)
{
	int fd;

	pc_begin("XSH.fsync.001");
	pc_require_space();
	fd = open("f", O_RDWR | O_CREAT, 0600);
	pc_check(fd >= 0, "cannot open f");
	pc_check(write(fd, "hello world", 11) == 11, "write failed");
	pc_test();
	pc_check(fsync(fd) == 0, "fsync() failed: %s", pc_errname(errno));
	pc_check(lseek(fd, 0, SEEK_CUR) == 11, "the file offset changed");
	pc_check(pc_file_size("f") == 11, "the length changed");
	pc_check(pc_file_equals("f", "hello world"), "the contents changed");
	pc_pass();
}
