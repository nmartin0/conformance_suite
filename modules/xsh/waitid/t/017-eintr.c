/*
 * A caught signal interrupts waitid().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.waitid.017
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* A child that lives ms milliseconds and then calls _exit(code). */
static pid_t kid(long ms, int code)
{
	pid_t r = fork();

	if (r == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (r == 0) {
		sleep_ms(ms);
		_exit(code);
	}
	return r;
}

/* Kills and reaps a child. */
static void bury(pid_t c)
{
	(void)kill(c, SIGKILL);
	(void)waitpid(c, NULL, 0);
}

static void on_alarm(int sig)
{
	(void)sig;
}

int main(void)
{
	struct sigaction sa;
	siginfo_t si;
	pid_t c;
	int r, e;

	pc_begin("XSH.waitid.017");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alarm;	/* no SA_RESTART */
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	c = kid(3000, 0);
	alarm(1);
	pc_test();
	r = waitid(P_PID, (id_t)c, &si, WEXITED);
	e = errno;
	alarm(0);
	bury(c);
	pc_check(r == -1, "waitid() returned %d instead of being interrupted by the signal", r);
	pc_check(e == EINTR, "waitid() failed with %s, expected EINTR", pc_errname(e));
	pc_pass();
}
