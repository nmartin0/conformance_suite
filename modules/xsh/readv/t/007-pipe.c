/*
 * readv() on a pipe returns what is there, in order.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.readv.007
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
	char a[3], b[3];
	const char *data = "wxyz";
	struct iovec iov[2];
	ssize_t r;
	int p[2], i;

	pc_begin("XSH.readv.007");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_check(write(p[1], data, 4) == 4, "write to the pipe failed");
	iov[0].iov_base = a; iov[0].iov_len = sizeof a;
	iov[1].iov_base = b; iov[1].iov_len = sizeof b;
	pc_test();
	r = readv(p[0], iov, 2);
	pc_check(r >= 1 && r <= 4, "readv() returned %ld", (long)r);
	for (i = 0; i < r; i++) {
		int inbuf = i < 3 ? a[i] : b[i - 3];

		pc_check(inbuf == data[i], "byte %d is %c, not %c", i, inbuf, data[i]);
	}
	pc_pass();
}
