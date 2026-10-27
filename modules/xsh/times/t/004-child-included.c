/*
 * A waited-for child's time joins tms_cutime and tms_cstime.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.times.004
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
/* Returns 0 if the real-time cap ended it before that much CPU time was used. */
static int burn_user(long ms)
{
	volatile unsigned long x = 1;
	clock_t end = clock() + (clock_t)((double)ms / 1000.0 * (double)CLOCKS_PER_SEC);
	long real0 = now_ms();
	unsigned long i;

	while (clock() < end && now_ms() - real0 < 30000)
		for (i = 0; i < 1000000; i++)
			x = x * 1664525UL + 1013904223UL;
	return clock() >= end;
}

/*
 * CPU time to spend, in ms: at least 0.5 s, and at least 20 clock ticks, so
 * that a coarse tick (sysconf(_SC_CLK_TCK) small) still shows growth of one
 * tick. A tick rate that cannot be read makes the test UNRESOLVED.
 */
static long burn_ms(void)
{
	long hz = sysconf(_SC_CLK_TCK), ms;

	if (hz <= 0)
		pc_unresolved("sysconf(_SC_CLK_TCK) gave %ld", hz);
	ms = (20L * 1000L + hz - 1) / hz;
	if (ms < 500)
		ms = 500;
	if (ms > 20000)
		ms = 20000;
	return ms;
}

int main(void)
{
	struct tms a, b;
	double d;
	long ms;
	pid_t c;
	int st;

	pc_begin("XSH.times.004");
	guard(90);
	pc_test();
	ms = burn_ms();
	(void)ticks(&a);
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0) {
		_exit(burn_user(ms) ? 0 : 95);
	}
	if (waitpid(c, &st, 0) != c)
		pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st) || WEXITSTATUS(st) != 0)
		pc_unresolved("the child did not use the CPU time the test needs (status %#x)", st);
	(void)ticks(&b);
	d = ((double)b.tms_cutime + (double)b.tms_cstime) - ((double)a.tms_cutime + (double)a.tms_cstime);
	pc_check(d >= 1, "tms_cutime + tms_cstime grew by %.0f ticks after waiting for a child that used at least 0.5 s of CPU time", d);
	pc_pass();
}
