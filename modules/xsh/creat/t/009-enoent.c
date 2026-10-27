/*
 * creat() with a missing directory or an empty path fails with ENOENT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creat.009
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

	pc_begin("XSH.creat.009");
	pc_test();
	pc_expect_err(creat("nodir/f", 0600), ENOENT, "creat(\"nodir/f\")");
	pc_expect_err(creat("", 0600), ENOENT, "creat(\"\")");
	pc_check(lstat("nodir", &st) == -1 && errno == ENOENT, "creat() created nodir");
	pc_pass();
}
