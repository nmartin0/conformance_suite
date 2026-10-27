/*
 * writev() on an O_APPEND descriptor writes everything at the end.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.writev.004
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

	pc_begin("XSH.writev.004");
	pc_make_file("f", "xx");
	fd = open("f", O_WRONLY | O_APPEND);
	pc_check(fd >= 0, "cannot open f");
	iov[0].iov_base = "ab"; iov[0].iov_len = 2;
	iov[1].iov_base = "cd"; iov[1].iov_len = 2;
	pc_test();
	pc_check(writev(fd, iov, 2) == 4, "writev() did not return 4");
	pc_check(pc_file_equals("f", "xxabcd"), "the data was not appended in order");
	pc_pass();
}
