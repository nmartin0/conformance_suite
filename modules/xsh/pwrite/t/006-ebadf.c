/*
 * pwrite() on a descriptor not open, or not open for writing, fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pwrite.006
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

int main(void)
{
	int bad, r;

	pc_begin("XSH.pwrite.006");
	pc_make_file("f", "data");
	r = open("f", O_RDONLY);
	pc_check(r >= 0, "cannot open f");
	bad = pc_lowest_free_fd();
	pc_test();
	pc_expect_err(pwrite(bad, "x", 1, 0), EBADF, "pwrite(<not open>)");
	pc_expect_err(pwrite(r, "x", 1, 0), EBADF, "pwrite() on a descriptor open for reading only");
	pc_pass();
}
