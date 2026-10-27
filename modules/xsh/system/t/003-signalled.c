/*
 * system() returns the status of a shell killed by a signal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.system.003
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
	int r;

	pc_begin("XSH.system.003");
	pc_test();
	r = system("kill -s KILL $$");
	pc_check(r != -1, "system() failed: %s", pc_errname(errno));
	pc_check(WIFSIGNALED(r), "status %#x of a shell that killed itself with SIGKILL is not a signal status", r);
	pc_check(WTERMSIG(r) == SIGKILL, "terminating signal %d, expected SIGKILL (%d)", WTERMSIG(r), SIGKILL);
	pc_pass();
}
