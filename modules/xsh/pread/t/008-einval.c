/*
 * pread() with a negative offset on a regular file fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pread.008
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
	char c;
	int fd;

	pc_begin("XSH.pread.008");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 3, SEEK_SET) == 3, "lseek failed");
	pc_test();
	pc_expect_err(pread(fd, &c, 1, -1), EINVAL, "pread() with offset -1");
	pc_check(lseek(fd, 0, SEEK_CUR) == 3, "the file offset changed");
	pc_pass();
}
