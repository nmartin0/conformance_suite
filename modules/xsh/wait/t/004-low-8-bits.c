/*
 * WEXITSTATUS keeps the low 8 bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.004
 * MUTANT    wait.status_garbage
 * VARIANT   wait.status_encoding
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	pid_t c;
	int st;

	pc_begin("XSH.wait.004");
	pc_test();
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed");
	if (c == 0)
		_exit(0x1ff);
	pc_check(waitpid(c, &st, 0) == c, "waitpid failed");
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 255, "WEXITSTATUS is %d, expected 255", WEXITSTATUS(st));
	pc_pass();
}
