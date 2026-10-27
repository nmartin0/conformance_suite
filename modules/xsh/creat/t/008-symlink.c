/*
 * creat() through a symbolic link truncates the file it names.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creat.008
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

	pc_begin("XSH.creat.008");
	pc_make_file("f", "contents");
	pc_make_symlink("f", "sl");
	pc_test();
	pc_expect_ok(creat("sl", 0600), "creat(\"sl\")");
	pc_check(pc_file_size("f") == 0, "the file f was not truncated");
	pc_check(lstat("sl", &st) == 0 && S_ISLNK(st.st_mode), "sl is no longer a symbolic link");
	pc_pass();
}
