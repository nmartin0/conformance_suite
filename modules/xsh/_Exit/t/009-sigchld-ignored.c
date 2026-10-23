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

static void on_alarm(int sig)
{
	(void)sig;
}

int main(void)
{
	struct sigaction sa;
	pid_t c;
	int st, r, e;

	pc_begin("XSH._Exit.009");
	memset(&sa, 0, sizeof sa);
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	pc_check(sigaction(SIGCHLD, &sa, NULL) == 0, "sigaction(SIGCHLD, SIG_IGN) failed");
	sa.sa_handler = on_alarm;	/* no SA_RESTART: a stuck wait() gives EINTR */
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction(SIGALRM) failed");
	pc_test();
	c = spawn();
	if (c == 0) {
		struct timespec ts = { 0, 300000000 };
		(void)nanosleep(&ts, NULL);	/* the parent is blocked in wait() by then */
		_exit(0);
	}
	alarm(10);
	errno = 0;
	r = wait(&st);
	e = errno;
	alarm(0);
	if (r == -1 && e == EINTR)
		pc_fail("wait() still blocked 10 s after the child's exit");
	pc_check(r == -1 && e == ECHILD,
	    "wait() for a child whose status is discarded did not fail with ECHILD (%s)", pc_errname(e));
	/* No zombie: the child's lifetime ended at once. */
	errno = 0;
	pc_check(kill(c, 0) == -1 && (errno == ESRCH || errno == EPERM),
	    "the child still exists after it terminated with SIGCHLD ignored");
	pc_pass();
}
