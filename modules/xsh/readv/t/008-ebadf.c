/*
 * readv() on a descriptor not open, or not open for reading, fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.readv.008
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
	int bad, w;

	pc_begin("XSH.readv.008");
	pc_make_file("f", "data");
	w = open("f", O_WRONLY);
	pc_check(w >= 0, "cannot open f");
	bad = pc_lowest_free_fd();
	iov[0].iov_base = &c; iov[0].iov_len = 1;
	pc_test();
	pc_expect_err(readv(bad, iov, 1), EBADF, "readv(<not open>)");
	pc_expect_err(readv(w, iov, 1), EBADF, "readv() on a descriptor open for writing only");
	pc_pass();
}
