/*
 * The other waiting thread then fails with ECHILD.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH._Exit.020
 * MUTANT    wait.not_consumed
 */
#include "pc.h"
#include <poll.h>
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
static int ready[2], fin[2];

/* Waits up to ms milliseconds for a waiter to finish: 1 if one did. */
static int wait_done(long ms)
{
	struct pollfd p;
	char c;

	p.fd = fin[0];
	p.events = POLLIN;
	return poll(&p, 1, (int)ms) == 1 && read(fin[0], &c, 1) == 1;
}

static void *waiter(void *a)
{
	struct res *x = a;

	(void)write(ready[1], "w", 1);
	while ((x->r = waitpid(-1, &x->st, 0)) == -1 && errno == EINTR)
		;
	x->e = x->r == -1 ? errno : 0;
	(void)write(fin[1], "d", 1);
	return NULL;
}

int main(void)
{
	pc_begin("XSH._Exit.020");
	{
		pthread_t t[2];
		pid_t kid;
		int i, go[2], got_pid = 0, got_echild = 0;
		char c;

		pc_check(pipe(ready) == 0 && pipe(go) == 0 && pipe(fin) == 0, "pipe failed");
		pc_test();
		kid = fork();
		if (kid == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (kid == 0) {
			(void)read(go[0], &c, 1);
			_exit(5);
		}
		for (i = 0; i < 2; i++)
			if (pthread_create(&t[i], NULL, waiter, &res[i]) != 0) {
				(void)kill(kid, SIGKILL);
				pc_unresolved("pthread_create failed");
			}
		for (i = 0; i < 2; i++)
			(void)read(ready[0], &c, 1);
		sleep_ms(200);
		(void)write(go[1], "g", 1);
		{
			/* The other waiter may stay blocked (Q-31): wait for it a while, but do not join it. */
			int ndone = wait_done(10000);

			if (ndone == 1)
				ndone += wait_done(2000);
		pc_check(ndone >= 1, "neither waiting thread returned within 10 s of the child's exit");
		pc_check(ndone == 2, "the thread that did not get the status is still blocked 2 s later");
		}
		for (i = 0; i < 2; i++) {
			if (res[i].r == kid && WIFEXITED(res[i].st) && WEXITSTATUS(res[i].st) == 5)
				got_pid++;
			else if (res[i].r == -1 && res[i].e == ECHILD)
				got_echild++;
		}

		pc_check(got_pid == 1, "%d threads got the child's status", got_pid);
		pc_check(got_echild == 1, "the other thread got %ld (%s), not ECHILD", (long)(res[0].r == kid ? res[1].r : res[0].r),
			 pc_errname(res[0].r == kid ? res[1].e : res[0].e));
		(void)got_echild;
	}
	pc_pass();
}
