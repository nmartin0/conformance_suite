/*
 * A child that did not wait does not count its own child.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.times.006
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
	pid_t c;
	int st;

	pc_begin("XSH.times.006");
	guard(90);
	pc_test();
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0) {
		struct tms b;
		pid_t g;
		char x;
		int p[2];

		if (pipe(p) != 0)
			_exit(90);
		g = fork();
		if (g == -1)
			_exit(91);
		if (g == 0) {
			(void)close(p[0]);
			burn_user(500);
			_exit(0);
		}
		(void)close(p[1]);
		while (read(p[0], &x, 1) == -1 && errno == EINTR)
			;
		sleep_ms(500);		/* the grandchild has ended, and is not waited for */
		if (times(&b) == (clock_t)-1)
			_exit(92);
		_exit(b.tms_cutime == 0 && b.tms_cstime == 0 ? 0 : 1);
	}
	if (waitpid(c, &st, 0) != c)
		pc_unresolved("waitpid failed: %s", pc_errname(errno));
	pc_check(WIFEXITED(st), "the child ended abnormally (status %#x)", st);
	if (WEXITSTATUS(st) >= 90)
		pc_unresolved("the child could not set up its own child (code %d)", WEXITSTATUS(st));
	pc_check(WEXITSTATUS(st) == 0, "a child that never waited for its own child had a non-zero tms_cutime or tms_cstime");
	pc_pass();
}
