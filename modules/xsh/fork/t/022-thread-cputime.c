/*
 * The child's thread CPU-time clock starts at zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.022
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

int main(void)
{
	pc_begin("XSH.fork.022");
	{
		struct timespec t;
		volatile unsigned long spin = 0;
		pid_t kid;
		int st;

		do {
			for (int i = 0; i < 1000000; i++)
				spin++;
			pc_check(clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t) == 0, "clock_gettime failed: %s", pc_errname(errno));
		} while (t.tv_sec == 0 && t.tv_nsec < 200000000);
		pc_test();
		kid = fork();
		if (kid == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (kid == 0) {
			struct timespec c;
			if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &c) != 0)
				_exit(99);
			_exit(c.tv_sec == 0 && c.tv_nsec < 100000000 ? 0 : 1);
		}
		pc_check(waitpid(kid, &st, 0) == kid && WIFEXITED(st), "the child did not exit");
		pc_check(WEXITSTATUS(st) != 99, "clock_gettime failed in the child");
		pc_check(WEXITSTATUS(st) == 0, "the child's thread CPU-time clock did not start at zero");
	}
	pc_pass();
}
