/*
 * With SA_NOCLDWAIT, wait() blocks until every child has ended.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaction.025
 * MUTANT    wait.nocldwait_no_block
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Waits up to ms milliseconds for *flag to become non-zero. */
static int wait_for(volatile sig_atomic_t *flag, long ms)
{
	long i;

	for (i = 0; i < ms / 10 && !*flag; i++)
		sleep_ms(10);
	return *flag != 0;
}

static long ms_since(const struct timespec *t0)
{
	struct timespec t;

	(void)clock_gettime(CLOCK_MONOTONIC, &t);
	return (long)(t.tv_sec - t0->tv_sec) * 1000 + (t.tv_nsec - t0->tv_nsec) / 1000000;
}

static void catcher(int s)
{
	(void)s;
}

int main(void)
{
	pc_begin("XSH.sigaction.025");
	{
		struct sigaction sa;
		struct timespec t0;
		pid_t a, b, r;
		long ms;
		int st;

		memset(&sa, 0, sizeof sa);
		sa.sa_handler = catcher;
		sa.sa_flags = SA_NOCLDWAIT;
		sigemptyset(&sa.sa_mask);
		pc_check(sigaction(SIGCHLD, &sa, NULL) == 0, "sigaction(SA_NOCLDWAIT) failed: %s", pc_errname(errno));
		pc_test();
		(void)clock_gettime(CLOCK_MONOTONIC, &t0);
		a = fork();
		if (a == 0)
			_exit(0);
		b = fork();
		if (b == 0) {
			sleep_ms(300);
			_exit(0);
		}
		if (a == -1 || b == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		while ((r = wait(&st)) == -1 && errno == EINTR)
			;
		ms = ms_since(&t0);
		pc_check(r == -1 && errno == ECHILD, "wait() returned %ld (%s)", (long)r, pc_errname(errno));
		pc_check(ms >= 250, "wait() failed with ECHILD after %ld ms, while a child was still running", ms);
	}
	pc_pass();
}
