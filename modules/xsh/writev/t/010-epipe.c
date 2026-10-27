/*
 * writev() to a pipe with no reader fails with EPIPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.writev.010
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
	struct iovec iov[1];
	int p[2];

	pc_begin("XSH.writev.010");
	pc_check(signal(SIGPIPE, SIG_IGN) != SIG_ERR, "signal failed");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_check(close(p[0]) == 0, "close failed");
	iov[0].iov_base = "x"; iov[0].iov_len = 1;
	pc_test();
	pc_expect_err(writev(p[1], iov, 1), EPIPE, "writev() to a pipe with no reader");
	pc_pass();
}
