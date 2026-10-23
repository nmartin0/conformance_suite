/*
 * _exit() runs no at_quick_exit() functions.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.013
 * MUTANT    _Exit.calls_quick
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

static int report_fd = -1;

static void handler(void)
{
	(void)write(report_fd, "h", 1);
}

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
	int p[2], st;
	pid_t c;
	char ch;

	pc_begin("XSH._Exit.013");
	pc_check(pipe(p) == 0, "pipe failed");
	report_fd = p[1];
	pc_test();
	c = spawn();
	if (c == 0) {
		(void)close(p[0]);
		if (at_quick_exit(handler) != 0)
			_exit(99);
		_exit(0);
	}
	(void)close(p[1]);
	st = reap(c);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child failed (%#x)", st);
	pc_check(read(p[0], &ch, 1) == 0, "a function registered with at_quick_exit() ran");
	pc_pass();
}
