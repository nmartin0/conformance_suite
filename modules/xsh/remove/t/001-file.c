/*
 * remove() of a regular file removes the name.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remove.001
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

	pc_begin("XSH.remove.001");
	pc_make_file("f", "data");
	pc_test();
	pc_check(remove("f") == 0, "remove() failed: %s", pc_errname(errno));
	pc_check(lstat("f", &st) == -1 && errno == ENOENT, "f still exists");
	pc_expect_err(open("f", O_RDONLY), ENOENT, "open() of the removed name");
	pc_pass();
}
