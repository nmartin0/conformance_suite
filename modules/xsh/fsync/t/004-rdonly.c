/*
 * fsync() of a read-only descriptor of a regular file returns 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fsync.004
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
	int fd;

	pc_begin("XSH.fsync.004");
	pc_make_file("f", "data");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	pc_check(fsync(fd) == 0, "fsync() failed: %s", pc_errname(errno));
	pc_pass();
}
