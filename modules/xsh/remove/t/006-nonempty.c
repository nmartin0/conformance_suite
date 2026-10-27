/*
 * remove() of a non-empty directory fails with EEXIST or ENOTEMPTY.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remove.006
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
	int r, e;

	pc_begin("XSH.remove.006");
	pc_make_dir("d");
	pc_make_file("d/x", "data");
	pc_test();
	r = remove("d");
	e = errno;
	pc_check(r == -1, "remove() of a non-empty directory succeeded");
	pc_check(e == EEXIST || e == ENOTEMPTY, "remove() failed with %s, not EEXIST or ENOTEMPTY", pc_errname(e));
	pc_check(stat("d", &st) == 0 && S_ISDIR(st.st_mode), "d is gone");
	pc_check(pc_file_equals("d/x", "data"), "d/x changed");
	pc_pass();
}
