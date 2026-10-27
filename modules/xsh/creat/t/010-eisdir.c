/*
 * creat() of a directory fails with EISDIR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creat.010
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

	pc_begin("XSH.creat.010");
	pc_make_dir("d");
	pc_test();
	pc_expect_err(creat("d", 0600), EISDIR, "creat(\"d\") of a directory");
	pc_check(stat("d", &st) == 0 && S_ISDIR(st.st_mode), "the directory is gone");
	pc_pass();
}
