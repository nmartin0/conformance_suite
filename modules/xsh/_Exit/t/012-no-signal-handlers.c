/*
 * _exit() runs no signal handlers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.012
 * MUTANT    _Exit.calls_atexit
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

static void on_usr1(int sig)
{
	(void)sig;
	(void)write(report_fd, "s", 1);
}

/* Registered with atexit(): if _exit() wrongly ran it, the signal would arrive. */
static void unblock(void)
{
	sigset_t s;
	sigemptyset(&s);
	sigaddset(&s, SIGUSR1);
	(void)sigprocmask(SIG_UNBLOCK, &s, NULL);
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
	int p[2], st;
	pid_t c;
	char ch;

	pc_begin("XSH._Exit.012");
	pc_check(pipe(p) == 0, "pipe failed");
	report_fd = p[1];
	pc_test();
	c = spawn();
	if (c == 0) {
		struct sigaction sa;
		sigset_t s;
		(void)close(p[0]);
		memset(&sa, 0, sizeof sa);
		sa.sa_handler = on_usr1;
		sigemptyset(&sa.sa_mask);
		sigemptyset(&s);
		sigaddset(&s, SIGUSR1);
		if (sigaction(SIGUSR1, &sa, NULL) != 0 || sigprocmask(SIG_BLOCK, &s, NULL) != 0 ||
		    raise(SIGUSR1) != 0 || atexit(unblock) != 0)
			_exit(99);
		_exit(0);
	}
	(void)close(p[1]);
	st = reap(c);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the child failed (%#x)", st);
	pc_check(read(p[0], &ch, 1) == 0, "a signal handler ran during _exit()");
	pc_pass();
}
