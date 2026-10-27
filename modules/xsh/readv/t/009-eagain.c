/*
 * readv() of an empty non-blocking pipe with a writer fails with EAGAIN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.readv.009
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
	char c;
	struct iovec iov[1];
	int p[2];

	pc_begin("XSH.readv.009");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_check(fcntl(p[0], F_SETFL, O_NONBLOCK) == 0, "F_SETFL failed");
	iov[0].iov_base = &c; iov[0].iov_len = 1;
	pc_test();
	pc_expect_err(readv(p[0], iov, 1), EAGAIN, "readv() of an empty non-blocking pipe");
	pc_pass();
}
