/*
 * writev() to a pipe delivers the buffers in order.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.writev.009
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
	char buf[8];
	int p[2];

	pc_begin("XSH.writev.009");
	pc_check(pipe(p) == 0, "pipe failed");
	iov[0].iov_base = "ab"; iov[0].iov_len = 2;
	iov[1].iov_base = "cd"; iov[1].iov_len = 2;
	pc_test();
	pc_check(writev(p[1], iov, 2) == 4, "writev() did not return 4");
	pc_check(read(p[0], buf, sizeof buf) == 4, "read() did not return 4 bytes");
	pc_check(memcmp(buf, "abcd", 4) == 0, "the reader got the wrong bytes");
	pc_pass();
}
