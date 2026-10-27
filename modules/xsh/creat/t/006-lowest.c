/*
 * creat() returns the lowest free descriptor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creat.006
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
	int a, b, low, fd;

	pc_begin("XSH.creat.006");
	pc_require_space();
	a = open(".", O_RDONLY);
	b = open(".", O_RDONLY);
	pc_check(a >= 0 && b >= 0, "cannot open descriptors");
	pc_check(close(a) == 0, "close failed");
	low = pc_lowest_free_fd();
	pc_check(low < b, "no gap below an open descriptor");
	pc_test();
	fd = creat("new", 0600);
	pc_expect_ok(fd, "creat(\"new\")");
	pc_check(fd == low, "creat() returned %d, not the lowest free descriptor %d", fd, low);
	pc_pass();
}
