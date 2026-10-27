/*
 * remove() of a path ending in dot fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remove.012
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

	pc_begin("XSH.remove.012");
	pc_make_dir("d");
	pc_make_dir("d/e");
	pc_test();
	pc_expect_err(remove("d/e/."), EINVAL, "remove(\"d/e/.\")");
	pc_check(remove("d/e/..") == -1, "remove(\"d/e/..\") succeeded");
	pc_check(stat("d", &st) == 0 && stat("d/e", &st) == 0, "a directory was removed");
	pc_pass();
}
