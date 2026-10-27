/*
 * remove() of one of two names leaves the other.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remove.003
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

	pc_begin("XSH.remove.003");
	pc_make_file("f", "data");
	if (link("f", "g") != 0)
		pc_unresolved("link failed: %s", pc_errname(errno));
	pc_test();
	pc_check(remove("g") == 0, "remove() failed: %s", pc_errname(errno));
	pc_check(lstat("g", &st) == -1 && errno == ENOENT, "g still exists");
	pc_check(stat("f", &st) == 0, "f is gone");
	pc_check(st.st_nlink == 1, "f has %ld links, not 1", (long)st.st_nlink);
	pc_check(pc_file_equals("f", "data"), "f changed");
	pc_pass();
}
