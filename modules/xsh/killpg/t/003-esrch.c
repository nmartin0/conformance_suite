/*
 * killpg() of a group that does not exist gives ESRCH.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.killpg.003
 * MUTANT    killpg.esrch_hidden
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
	pc_begin("XSH.killpg.003");
	{
		pid_t k;
		int p[2], st;
		char c;

		pc_check(pipe(p) == 0, "pipe failed");
		k = fork();
		if (k == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (k == 0) {
			(void)close(p[1]);
			(void)read(p[0], &c, 1);
			_exit(0);
		}
		(void)close(p[0]);
		pc_test();
		pc_expect_err(killpg(k, 0), ESRCH, "killpg(no such group, 0)");
		pc_expect_err(killpg(k, SIGUSR1), ESRCH, "killpg(no such group, SIGUSR1)");
		(void)close(p[1]);
		(void)waitpid(k, &st, 0);
	}
	pc_pass();
}
