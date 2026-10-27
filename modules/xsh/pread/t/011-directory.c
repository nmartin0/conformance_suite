/*
 * pread() on a directory reads or fails with EISDIR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pread.011
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
	char buf[64];
	ssize_t r;
	int fd, e;

	pc_begin("XSH.pread.011");
	fd = open(".", O_RDONLY);
	pc_check(fd >= 0, "cannot open the directory");
	pc_test();
	r = pread(fd, buf, sizeof buf, 0);
	e = errno;
	if (r >= 0)
		pc_pass();
	pc_check(e == EISDIR, "pread() on a directory failed with %s, not EISDIR", pc_errname(e));
	pc_pass();
}
