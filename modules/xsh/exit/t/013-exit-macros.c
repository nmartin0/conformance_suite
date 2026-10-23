/*
 * exit(EXIT_SUCCESS) and exit(EXIT_FAILURE).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.exit.013
 * MUTANT    exit.status_wrong
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
	if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
		pc_unresolved("the child could not set up");
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

	pc_begin("XSH.exit.013");
	pc_test();
	c = spawn();
	if (c == 0)
		exit(EXIT_SUCCESS);
	st = reap(c);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "EXIT_SUCCESS gave status %#x", st);
	c = spawn();
	if (c == 0)
		exit(EXIT_FAILURE);
	st = reap(c);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) != 0, "EXIT_FAILURE gave status %#x", st);
	pc_pass();
}
