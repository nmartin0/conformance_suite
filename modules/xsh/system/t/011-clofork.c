/*
 * FD_CLOFORK descriptors are closed in the command.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.system.011
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
#ifndef FD_CLOFORK
	pc_begin("XSH.system.011");
	pc_unresolved("<fcntl.h> does not define FD_CLOFORK");
#else
	char cmd[128];
	int a, b, r;

	pc_begin("XSH.system.011");
	pc_make_file("f", "x");
	a = open("f", O_RDONLY);
	b = open("f", O_RDONLY);
	pc_check(a >= 0 && b >= 0, "cannot open f");
	pc_check(fcntl(a, F_SETFD, FD_CLOFORK) == 0, "cannot set FD_CLOFORK");
	pc_test();
	(void)snprintf(cmd, sizeof cmd, "true 2>/dev/null <&%d", b);
	r = system(cmd);
	pc_check(r != -1 && WIFEXITED(r), "system() gave %d", r);
	pc_check(WEXITSTATUS(r) == 0, "the descriptor without FD_CLOFORK was not open in the command (status %d)", WEXITSTATUS(r));
	(void)snprintf(cmd, sizeof cmd, "true 2>/dev/null <&%d", a);
	r = system(cmd);
	pc_check(r != -1 && WIFEXITED(r), "system() gave %d", r);
	pc_check(WEXITSTATUS(r) != 0, "the descriptor with FD_CLOFORK was still open in the command");
	pc_pass();
#endif
}
