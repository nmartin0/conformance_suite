/*
 * writev() with every length 0 on a regular file does nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.writev.003
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

int main(void)
{
	struct iovec iov[2];
	int fd;

	pc_begin("XSH.writev.003");
	pc_make_file("f", "abc");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 1, SEEK_SET) == 1, "lseek failed");
	iov[0].iov_base = "x"; iov[0].iov_len = 0;
	iov[1].iov_base = "y"; iov[1].iov_len = 0;
	pc_test();
	pc_check(writev(fd, iov, 2) == 0, "writev() did not return 0");
	pc_check(pc_file_equals("f", "abc"), "the file changed");
	pc_check(lseek(fd, 0, SEEK_CUR) == 1, "the file offset changed");
	pc_pass();
}
