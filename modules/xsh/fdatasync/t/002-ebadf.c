/*
 * fdatasync() of a descriptor that is not open fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fdatasync.002
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

	pc_begin("XSH.fdatasync.002");
	bad = pc_lowest_free_fd();
	pc_test();
	pc_expect_err(fdatasync(bad), EBADF, "fdatasync(<not open>)");
	pc_expect_err(fdatasync(-1), EBADF, "fdatasync(-1)");
	pc_pass();
}
