/*
 * fsync() of a descriptor that is not open fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fsync.002
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
	int bad;

	pc_begin("XSH.fsync.002");
	bad = pc_lowest_free_fd();
	pc_test();
	pc_expect_err(fsync(bad), EBADF, "fsync(<not open>)");
	pc_expect_err(fsync(-1), EBADF, "fsync(-1)");
	pc_pass();
}
