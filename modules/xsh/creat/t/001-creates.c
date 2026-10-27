/*
 * creat() of a missing path creates a regular file of length 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creat.001
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
	struct stat st;
	int fd;

	pc_begin("XSH.creat.001");
	pc_require_space();
	pc_test();
	fd = creat("new", 0600);
	pc_expect_ok(fd, "creat(\"new\")");
	pc_check(lstat("new", &st) == 0, "file \"new\" does not exist");
	pc_check(S_ISREG(st.st_mode), "\"new\" is not a regular file");
	pc_check(st.st_size == 0, "\"new\" has length %ld", (long)st.st_size);
	pc_pass();
}
