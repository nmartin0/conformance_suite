/*
 * fsync() through another descriptor of the file returns 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fsync.006
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
	int w, r;

	pc_begin("XSH.fsync.006");
	pc_require_space();
	w = open("f", O_WRONLY | O_CREAT, 0600);
	pc_check(w >= 0, "cannot create f");
	pc_check(write(w, "abcdef", 6) == 6, "write failed");
	r = open("f", O_RDWR);
	pc_check(r >= 0, "cannot reopen f");
	pc_test();
	pc_check(fsync(r) == 0, "fsync() failed: %s", pc_errname(errno));
	pc_check(pc_file_equals("f", "abcdef"), "the contents changed");
	pc_pass();
}
