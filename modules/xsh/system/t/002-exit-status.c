/*
 * system() returns a wait status.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.system.002
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

static void expect(const char *cmd, int code)
{
	int r = system(cmd);

	pc_check(r != -1, "system(\"%s\") failed: %s", cmd, pc_errname(errno));
	pc_check(WIFEXITED(r), "system(\"%s\") returned %#x, which is not a normal exit status", cmd, r);
	pc_check(WEXITSTATUS(r) == code, "system(\"%s\") gave exit status %d, expected %d", cmd, WEXITSTATUS(r), code);
}

int main(void)
{
	pc_begin("XSH.system.002");
	pc_test();
	expect("exit 0", 0);
	expect("exit 42", 42);
	expect("exit 1", 1);
	pc_pass();
}
