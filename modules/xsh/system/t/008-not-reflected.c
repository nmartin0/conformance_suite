/*
 * Changes made by the command do not reach the caller.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.system.008
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

int main(void)
{
	char before[PATH_MAX], after[PATH_MAX];
	int r;

	pc_begin("XSH.system.008");
	pc_make_dir("sub");
	pc_check(setenv("PC_CHANGE", "orig", 1) == 0, "setenv failed");
	pc_check(getcwd(before, sizeof before) != NULL, "getcwd failed");
	pc_test();
	r = system("cd sub; PC_CHANGE=new; export PC_CHANGE; PC_ADDED=1; export PC_ADDED; exit 0");
	pc_check(r != -1 && WIFEXITED(r) && WEXITSTATUS(r) == 0, "system() gave %d", r);
	pc_check(getcwd(after, sizeof after) != NULL, "getcwd failed");
	pc_check(strcmp(before, after) == 0, "the working directory changed from %s to %s", before, after);
	pc_check(getenv("PC_CHANGE") != NULL && strcmp(getenv("PC_CHANGE"), "orig") == 0, "PC_CHANGE is now \"%s\"",
	    getenv("PC_CHANGE") ? getenv("PC_CHANGE") : "(unset)");
	pc_check(getenv("PC_ADDED") == NULL, "PC_ADDED became set in the caller");
	pc_pass();
}
