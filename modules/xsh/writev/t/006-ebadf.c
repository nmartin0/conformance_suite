/*
 * writev() on a descriptor not open, or not open for writing, fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.writev.006
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
	int bad, r;

	pc_begin("XSH.writev.006");
	pc_make_file("f", "data");
	r = open("f", O_RDONLY);
	pc_check(r >= 0, "cannot open f");
	bad = pc_lowest_free_fd();
	iov[0].iov_base = "x"; iov[0].iov_len = 1;
	pc_test();
	pc_expect_err(writev(bad, iov, 1), EBADF, "writev(<not open>)");
	pc_expect_err(writev(r, iov, 1), EBADF, "writev() on a descriptor open for reading only");
	pc_pass();
}
