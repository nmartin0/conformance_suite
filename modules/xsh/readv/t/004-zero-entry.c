/*
 * A zero-length buffer takes no data.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.readv.004
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
	char a[2], z[1], c[3];
	struct iovec iov[3];
	int fd;

	pc_begin("XSH.readv.004");
	pc_make_file("f", "abcdef");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	iov[0].iov_base = a; iov[0].iov_len = sizeof a;
	iov[1].iov_base = z; iov[1].iov_len = 0;
	iov[2].iov_base = c; iov[2].iov_len = sizeof c;
	pc_test();
	pc_check(readv(fd, iov, 3) == 5, "readv() did not return 5");
	pc_check(memcmp(a, "ab", 2) == 0 && memcmp(c, "cde", 3) == 0, "the data is wrong");
	pc_pass();
}
