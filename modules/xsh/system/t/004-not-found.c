/*
 * A command that is not found gives status 127.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.system.004
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

	pc_begin("XSH.system.004");
	pc_test();
	r = system("pc_no_such_utility_9f3a2c 2>/dev/null");
	pc_check(r != -1, "system() failed: %s", pc_errname(errno));
	pc_check(WIFEXITED(r), "status %#x is not a normal exit status", r);
	pc_check(WEXITSTATUS(r) == 127, "exit status %d, expected 127", WEXITSTATUS(r));
	pc_pass();
}
