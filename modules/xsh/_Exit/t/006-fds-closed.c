/*
 * Exiting closes the process's descriptors.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.006
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
	int p[2], st;
	pid_t c;
	char ch;

	pc_begin("XSH._Exit.006");
	pc_check(pipe(p) == 0, "pipe failed");
	alarm(10);
	pc_test();
	c = spawn();
	if (c == 0) {
		(void)close(p[0]);
		_exit(0);	/* still holding p[1] */
	}
	(void)close(p[1]);
	pc_check(read(p[0], &ch, 1) == 0, "the reader did not see end of file after the writer exited");
	st = reap(c);
	(void)st;
	pc_pass();
}
