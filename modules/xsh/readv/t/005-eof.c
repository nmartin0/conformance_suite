/*
 * readv() at the end of file returns 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.readv.005
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
	char a[4] = "zzzz";
	struct iovec iov[1];
	int fd;

	pc_begin("XSH.readv.005");
	pc_make_file("f", "abc");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 0, SEEK_END) == 3, "lseek failed");
	iov[0].iov_base = a; iov[0].iov_len = sizeof a;
	pc_test();
	pc_check(readv(fd, iov, 1) == 0, "readv() at the end of file did not return 0");
	pc_check(memcmp(a, "zzzz", 4) == 0, "the buffer changed");
	pc_pass();
}
