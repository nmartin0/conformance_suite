/*
 * Without SA_SIGINFO, a queued signal is delivered at least once.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigqueue.009
 * MUTANT    sigqueue.drops_plain
 * VARIANT   sigqueue.plain_once
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

static volatile sig_atomic_t n;

static void h(int s)
{
	(void)s;
	n++;
}

int main(void)
{
	pc_begin("XSH.sigqueue.009");
	{
		struct sigaction sa;
		sigset_t one;
		union sigval v;

		memset(&sa, 0, sizeof sa);
		sa.sa_handler = h;
		sigemptyset(&sa.sa_mask);
		pc_check(sigaction(SIGRTMIN, &sa, NULL) == 0, "sigaction failed");
		sigemptyset(&one);
		sigaddset(&one, SIGRTMIN);
		pc_check(sigprocmask(SIG_BLOCK, &one, NULL) == 0, "sigprocmask failed");
		v.sival_int = 5;
		pc_test();
		pc_check(sigqueue(getpid(), SIGRTMIN, v) == 0, "the first sigqueue() failed: %s", pc_errname(errno));
		pc_check(sigqueue(getpid(), SIGRTMIN, v) == 0, "the second sigqueue() failed: %s", pc_errname(errno));
		pc_check(sigprocmask(SIG_UNBLOCK, &one, NULL) == 0, "sigprocmask failed");
		pc_check(wait_for(&n, 2000), "the handler never ran");
	}
	pc_pass();
}
