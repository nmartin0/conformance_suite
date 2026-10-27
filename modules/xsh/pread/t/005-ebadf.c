/*
 * pread() on a descriptor not open, or not open for reading, fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pread.005
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
	char c;
	int bad, w;

	pc_begin("XSH.pread.005");
	pc_make_file("f", "data");
	w = open("f", O_WRONLY);
	pc_check(w >= 0, "cannot open f");
	bad = pc_lowest_free_fd();
	pc_test();
	pc_expect_err(pread(bad, &c, 1, 0), EBADF, "pread(<not open>)");
	pc_expect_err(pread(w, &c, 1, 0), EBADF, "pread() on a descriptor open for writing only");
	pc_pass();
}
