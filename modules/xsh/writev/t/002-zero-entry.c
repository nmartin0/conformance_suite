/*
 * A zero-length buffer writes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.writev.002
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
	struct iovec iov[3];
	int fd;

	pc_begin("XSH.writev.002");
	pc_require_space();
	fd = open("f", O_WRONLY | O_CREAT, 0600);
	pc_check(fd >= 0, "cannot create f");
	iov[0].iov_base = "ab"; iov[0].iov_len = 2;
	iov[1].iov_base = "zz"; iov[1].iov_len = 0;
	iov[2].iov_base = "cde"; iov[2].iov_len = 3;
	pc_test();
	pc_check(writev(fd, iov, 3) == 5, "writev() did not return 5");
	pc_check(pc_file_equals("f", "abcde"), "the file does not hold abcde");
	pc_pass();
}
