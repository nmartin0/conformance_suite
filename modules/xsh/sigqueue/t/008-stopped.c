/*
 * sigqueue() to a stopped process returns at once.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigqueue.008
 * MUTANT    sigqueue.waits_stopped
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

static pid_t kid;

static void release(int s)
{
	(void)s;
	(void)kill(kid, SIGCONT);
}

int main(void)
{
	pc_begin("XSH.sigqueue.008");
	{
		struct sigaction sa;
		struct timespec t0;
		union sigval v;
		long ms;
		int st, r;

		kid = fork();
		if (kid == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (kid == 0)
			for (;;)
				pause();
		pc_check(kill(kid, SIGSTOP) == 0, "kill(SIGSTOP) failed");
		pc_check(waitpid(kid, &st, WUNTRACED) == kid && WIFSTOPPED(st), "the child did not stop");
		memset(&sa, 0, sizeof sa);
		sa.sa_handler = release;
		sigemptyset(&sa.sa_mask);
		pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
		v.sival_int = 1;
		pc_test();
		(void)alarm(2);
		(void)clock_gettime(CLOCK_MONOTONIC, &t0);
		r = sigqueue(kid, SIGRTMIN, v);
		ms = ms_since(&t0);
		(void)alarm(0);
		(void)kill(kid, SIGKILL);
		(void)waitpid(kid, &st, 0);
		pc_check(r == 0, "sigqueue() to the stopped child failed: %s", pc_errname(errno));
		pc_check(ms < 500, "sigqueue() took %ld ms to return", ms);
	}
	pc_pass();
}
