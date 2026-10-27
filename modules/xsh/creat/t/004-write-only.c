/*
 * The descriptor from creat() is write-only, whatever the mode.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creat.004
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
	int fd;

	pc_begin("XSH.creat.004");
	pc_require_space();
	pc_test();
	fd = creat("ro", 0444);
	pc_expect_ok(fd, "creat(\"ro\", 0444)");
	pc_check(write(fd, "x", 1) == 1, "write() through the descriptor failed: %s", pc_errname(errno));
	pc_expect_err(read(fd, &c, 1), EBADF, "read() on the descriptor from creat()");
	pc_pass();
}
