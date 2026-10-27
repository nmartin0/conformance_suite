/*
 * writev() writes the buffers in array order and advances the file offset.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.writev.001
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

	pc_begin("XSH.writev.001");
	pc_require_space();
	fd = open("f", O_WRONLY | O_CREAT, 0600);
	pc_check(fd >= 0, "cannot create f");
	iov[0].iov_base = "ab"; iov[0].iov_len = 2;
	iov[1].iov_base = "cde"; iov[1].iov_len = 3;
	iov[2].iov_base = "f"; iov[2].iov_len = 1;
	pc_test();
	pc_check(writev(fd, iov, 3) == 6, "writev() did not return 6");
	pc_check(pc_file_equals("f", "abcdef"), "the file does not hold abcdef");
	pc_check(lseek(fd, 0, SEEK_CUR) == 6, "the file offset is not 6");
	iov[0].iov_base = "g"; iov[0].iov_len = 1;
	iov[1].iov_base = "h"; iov[1].iov_len = 1;
	pc_check(writev(fd, iov, 2) == 2, "second writev() did not return 2");
	pc_check(pc_file_equals("f", "abcdefgh"), "the second writev() did not continue after the first");
	pc_pass();
}
