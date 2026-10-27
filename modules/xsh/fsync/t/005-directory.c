/*
 * fsync() of a directory returns 0 or fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fsync.005
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
	int fd, r, e;

	pc_begin("XSH.fsync.005");
	pc_make_dir("d");
	pc_make_file("d/f", "x");
	fd = open("d", O_RDONLY);
	pc_check(fd >= 0, "cannot open d");
	pc_test();
	r = fsync(fd);
	e = errno;
	if (r == 0)
		pc_pass();
	pc_check(r == -1, "fsync() returned %d", r);
	pc_check(e == EINVAL, "fsync() of a directory failed with %s, not EINVAL", pc_errname(e));
	pc_pass();
}
