/*
 * _exit() and _Exit() behave the same.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.003
 * MUTANT    _Exit.calls_atexit
 * MUTANT    _Exit.status_wrong
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
	int p[2], how, st;
	pid_t c;
	char ch;

	pc_begin("XSH._Exit.003");
	pc_check(pipe(p) == 0, "pipe failed");
	report_fd = p[1];
	pc_test();
	for (how = 0; how < 2; how++) {
		c = spawn();
		if (c == 0) {
			(void)close(p[0]);
			if (atexit(handler) != 0)
				_exit(99);
			if (how == 0)
				_exit(17);
			_Exit(17);
		}
		st = reap(c);
		pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 17, "%s gave status %#x, expected 17",
		    how ? "_Exit(17)" : "_exit(17)", st);
	}
	(void)close(p[1]);
	pc_check(read(p[0], &ch, 1) == 0, "an atexit() handler ran");
	pc_pass();
}
