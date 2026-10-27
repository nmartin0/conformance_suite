/*
 * remove() of an empty directory removes it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remove.002
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

	pc_begin("XSH.remove.002");
	pc_make_dir("d");
	pc_test();
	pc_check(remove("d") == 0, "remove() failed: %s", pc_errname(errno));
	pc_check(lstat("d", &st) == -1 && errno == ENOENT, "d still exists");
	pc_pass();
}
