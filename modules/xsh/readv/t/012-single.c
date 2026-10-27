/*
 * readv() with one buffer reads like read().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.readv.012
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
	char a[4];
	struct iovec iov[1];
	int fd;

	pc_begin("XSH.readv.012");
	pc_make_file("f", "abcdef");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	iov[0].iov_base = a; iov[0].iov_len = sizeof a;
	pc_test();
	pc_check(readv(fd, iov, 1) == 4, "readv() did not return 4");
	pc_check(memcmp(a, "abcd", 4) == 0, "the data is wrong");
	pc_check(lseek(fd, 0, SEEK_CUR) == 4, "the file offset is not 4");
	pc_pass();
}
