/*
 * A terminating child sends SIGCHLD to its parent.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.007
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

static volatile sig_atomic_t got;

static void on_chld(int sig)
{
	(void)sig;
	got = 1;
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
	struct sigaction sa;
	pid_t c;
	int st;

	pc_begin("XSH._Exit.007");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_chld;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGCHLD, &sa, NULL) == 0, "sigaction failed");
	pc_test();
	c = spawn();
	if (c == 0)
		_exit(0);
	st = reap(c);
	(void)st;
	{
		struct timespec ts = { 0, 10000000 };
		int i;
		for (i = 0; i < 100 && !got; i++)
			(void)nanosleep(&ts, NULL);
	}
	pc_check(got, "SIGCHLD was not delivered to the parent");
	pc_pass();
}
