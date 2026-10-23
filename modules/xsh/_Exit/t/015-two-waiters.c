/*
 * Of two threads waiting for a child, exactly one gets its status.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.015
 * MUTANT    wait.not_consumed
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

struct res {
	pid_t r;
	int st, e;
};

static struct res res[2];

static void *waiter(void *a)
{
	struct res *x = a;

	while ((x->r = waitpid(-1, &x->st, 0)) == -1 && errno == EINTR)
		;
	x->e = x->r == -1 ? errno : 0;
	return NULL;
}

int main(void)
{
	pc_begin("XSH._Exit.015");
	{
		pthread_t t[2];
		pid_t kid;
		int i, got_pid = 0, got_echild = 0;

		pc_test();
		kid = fork();
		if (kid == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (kid == 0) {
			sleep_ms(300);
			_exit(5);
		}
		for (i = 0; i < 2; i++)
			pc_check(pthread_create(&t[i], NULL, waiter, &res[i]) == 0, "pthread_create failed");
		for (i = 0; i < 2; i++)
			pc_check(pthread_join(t[i], NULL) == 0, "pthread_join failed");
		for (i = 0; i < 2; i++) {
			if (res[i].r == kid && WIFEXITED(res[i].st) && WEXITSTATUS(res[i].st) == 5)
				got_pid++;
			else if (res[i].r == -1 && res[i].e == ECHILD)
				got_echild++;
		}
		pc_check(got_pid == 1 && got_echild == 1, "the threads got: %ld (%s), %ld (%s)", (long)res[0].r,
			 pc_errname(res[0].e), (long)res[1].r, pc_errname(res[1].e));
	}
	pc_pass();
}
