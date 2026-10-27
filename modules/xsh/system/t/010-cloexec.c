/*
 * FD_CLOEXEC descriptors are closed in the command.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.system.010
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
	char cmd[128];
	int a, b, r;

	pc_begin("XSH.system.010");
	pc_make_file("f", "x");
	a = open("f", O_RDONLY | O_CLOEXEC);
	b = open("f", O_RDONLY);
	pc_check(a >= 0 && b >= 0, "cannot open f");
	pc_test();
	/* a redirection from a descriptor that is not open is an error and gives a non-zero status */
	(void)snprintf(cmd, sizeof cmd, "true 2>/dev/null <&%d", b);
	r = system(cmd);
	pc_check(r != -1 && WIFEXITED(r), "system() gave %d", r);
	pc_check(WEXITSTATUS(r) == 0, "the descriptor without FD_CLOEXEC was not open in the command (status %d)", WEXITSTATUS(r));
	(void)snprintf(cmd, sizeof cmd, "true 2>/dev/null <&%d", a);
	r = system(cmd);
	pc_check(r != -1 && WIFEXITED(r), "system() gave %d", r);
	pc_check(WEXITSTATUS(r) != 0, "the descriptor with FD_CLOEXEC was still open in the command");
	pc_pass();
}
