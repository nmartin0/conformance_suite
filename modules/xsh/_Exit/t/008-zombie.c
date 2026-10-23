/*
 * A child is a zombie until waited for.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.008
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

	pc_begin("XSH._Exit.008");
	pc_test();
	c = spawn();
	if (c == 0)
		_exit(0);
	{
		struct timespec ts = { 0, 300000000 };
		(void)nanosleep(&ts, NULL);	/* the child has exited by now */
	}
	pc_check(kill(c, 0) == 0, "the terminated, unwaited child does not exist (%s)", pc_errname(errno));
	st = reap(c);
	(void)st;
	errno = 0;
	pc_check(kill(c, 0) == -1 && errno == ESRCH, "the child still exists after its status was obtained");
	pc_pass();
}
