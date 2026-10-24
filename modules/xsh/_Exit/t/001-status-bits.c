/*
 * wait() sees the low 8 bits of the _exit() status.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.001
 * MUTANT    _Exit.status_wrong
 * VARIANT   wait.status_encoding
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* Waits for pid and returns its raw status; anything odd is UNRESOLVED. */
static int reap(pid_t pid)
{
	int st;

	if (waitpid(pid, &st, 0) != pid)
		pc_unresolved("waitpid failed");
	return st;
}

static pid_t spawn(void)
{
	pid_t r = fork();

	if (r == -1)
		pc_unresolved("fork failed");
	return r;
}

int main(void)
{
	pid_t c;
	int st;

	pc_begin("XSH._Exit.001");
	pc_test();
	c = spawn();
	if (c == 0)
		_exit(0x1234);
	st = reap(c);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0x34, "WEXITSTATUS is %#x, expected 0x34", WEXITSTATUS(st));
	pc_pass();
}
