/*
 * A terminated child not yet waited for is not counted.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.times.005
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
#include <sys/times.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* A call that blocks must fail the test, not hang it. */
static void on_guard(int sig)
{
	(void)sig;
	pc_fail("blocked: the call under test did not finish in time");
}

static void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_guard;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(secs);
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

static long now_ms(void)
{
	struct timespec t;

	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec * 1000L + t.tv_nsec / 1000000L;
}

/* One reading of times(); an overflow (allowed) makes the test UNRESOLVED, any other failure is a FAIL. */
static clock_t ticks(struct tms *b)
{
	clock_t t;

	errno = 0;
	t = times(b);
	if (t == (clock_t)-1) {
		if (errno == EOVERFLOW)
			pc_unresolved("times() reported EOVERFLOW (allowed)");
		pc_fail("times() failed: %s", pc_errname(errno));
	}
	return t;
}

/* Spends about ms of CPU time (by clock()) on arithmetic: nearly all of it in user mode. */
static void burn_user(long ms)
{
	volatile unsigned long x = 1;
	clock_t end = clock() + (clock_t)((double)ms / 1000.0 * (double)CLOCKS_PER_SEC);
	long real0 = now_ms();
	unsigned long i;

	while (clock() < end && now_ms() - real0 < 30000)
		for (i = 0; i < 1000000; i++)
			x = x * 1664525UL + 1013904223UL;
}

int main(void)
{
	struct tms b;
	char x;
	pid_t c;
	int p[2], st;

	pc_begin("XSH.times.005");
	guard(90);
	pc_check(pipe(p) == 0, "pipe failed");
	pc_test();
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0) {
		(void)close(p[0]);
		burn_user(500);
		_exit(0);	/* closes p[1] */
	}
	(void)close(p[1]);
	while (read(p[0], &x, 1) == -1 && errno == EINTR)
		;
	sleep_ms(500);		/* the child has ended by now, and is not waited for */
	(void)ticks(&b);
	pc_check(b.tms_cutime == 0 && b.tms_cstime == 0, "tms_cutime %.0f and tms_cstime %.0f before the child was waited for, expected 0",
	    (double)b.tms_cutime, (double)b.tms_cstime);
	if (waitpid(c, &st, 0) != c)
		pc_unresolved("waitpid failed: %s", pc_errname(errno));
	pc_pass();
}
