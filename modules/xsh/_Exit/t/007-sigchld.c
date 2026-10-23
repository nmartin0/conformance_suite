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
	struct timespec limit = { 10, 0 };
	sigset_t set;
	siginfo_t si;
	pid_t c;
	int st, r;

	pc_begin("XSH._Exit.007");
	/*
	 * Caught, so that a blocked SIGCHLD must stay pending (a blocked
	 * signal whose action is to ignore may be discarded, XSH 2.4.1);
	 * blocked, so that it is accepted with sigtimedwait() instead of
	 * interrupting the wait.
	 */
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_chld;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGCHLD, &sa, NULL) == 0, "sigaction failed");
	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	pc_check(sigprocmask(SIG_BLOCK, &set, NULL) == 0, "sigprocmask failed");
	pc_test();
	c = spawn();
	if (c == 0)
		_exit(0);
	memset(&si, 0, sizeof si);
	r = sigtimedwait(&set, &si, &limit);
	pc_check(r == SIGCHLD, "no SIGCHLD within 10 s of the child's _exit() (%s)",
	    r == -1 ? pc_errname(errno) : "another signal");
	st = reap(c);
	(void)st;
	pc_pass();
}
