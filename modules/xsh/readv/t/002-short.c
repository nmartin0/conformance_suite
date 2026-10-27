/*
 * readv() with fewer bytes available than the buffers hold.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.readv.002
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
	char a[3], b[4], c[4];
	struct iovec iov[3];
	int fd;

	pc_begin("XSH.readv.002");
	pc_make_file("f", "12345");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	iov[0].iov_base = a; iov[0].iov_len = sizeof a;
	iov[1].iov_base = b; iov[1].iov_len = sizeof b;
	iov[2].iov_base = c; iov[2].iov_len = sizeof c;
	pc_test();
	pc_check(readv(fd, iov, 3) == 5, "readv() did not return 5");
	pc_check(memcmp(a, "123", 3) == 0, "first buffer is wrong");
	pc_check(memcmp(b, "45", 2) == 0, "second buffer does not start with the last two bytes");
	pc_pass();
}
