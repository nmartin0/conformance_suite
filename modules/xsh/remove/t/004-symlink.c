/*
 * remove() of a symbolic link removes the link, not what it names.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remove.004
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

	pc_begin("XSH.remove.004");
	pc_make_file("f", "data");
	pc_make_symlink("f", "sl");
	pc_make_symlink("nowhere", "dang");
	pc_test();
	pc_check(remove("sl") == 0, "remove(\"sl\") failed: %s", pc_errname(errno));
	pc_check(lstat("sl", &st) == -1 && errno == ENOENT, "sl still exists");
	pc_check(pc_file_equals("f", "data"), "f was changed or removed");
	pc_check(remove("dang") == 0, "remove() of a dangling link failed: %s", pc_errname(errno));
	pc_check(lstat("dang", &st) == -1 && errno == ENOENT, "the dangling link still exists");
	pc_pass();
}
