/*
 * pwrite() with a negative offset on a regular file fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pwrite.009
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

	pc_begin("XSH.pwrite.009");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 3, SEEK_SET) == 3, "lseek failed");
	pc_test();
	pc_expect_err(pwrite(fd, "x", 1, -1), EINVAL, "pwrite() with offset -1");
	pc_check(lseek(fd, 0, SEEK_CUR) == 3, "the file offset changed");
	pc_pass();
}
