/*
 * XSI: with SIGCHLD ignored, status is discarded.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.009
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

	pc_begin("XSH._Exit.009");
	signal(SIGCHLD, SIG_IGN);
	alarm(10);
	pc_test();
	c = spawn();
	if (c == 0) {
		struct timespec ts = { 0, 300000000 };
		(void)nanosleep(&ts, NULL);	/* the parent is blocked in wait() by then */
		_exit(0);
	}
	errno = 0;
	pc_check(wait(&st) == -1 && errno == ECHILD,
	    "wait() for a child whose status is discarded did not fail with ECHILD (%s)", pc_errname(errno));
	pc_pass();
}
