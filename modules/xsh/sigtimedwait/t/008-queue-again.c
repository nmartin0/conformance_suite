/*
 * Accepting queued signals frees room in the queue.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigtimedwait.008
 * MUTANT    sigqueue.eagain_sticky
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
	pc_begin("XSH.sigtimedwait.008");
	{
		struct timespec zero = { 0, 0 };
		siginfo_t info;
		sigset_t one;
		union sigval v;
		long max = sysconf(_SC_SIGQUEUE_MAX), i, taken = 0;

		if (max < 1 || max > 100000)
			pc_notinuse("{SIGQUEUE_MAX} is %ld here", max);
		sigemptyset(&one);
		sigaddset(&one, SIGRTMIN);
		pc_check(sigprocmask(SIG_BLOCK, &one, NULL) == 0, "sigprocmask failed");
		v.sival_int = 0;
		pc_test();
		for (i = 0; i < max + 10; i++)
			if (sigqueue(getpid(), SIGRTMIN, v) != 0)
				break;
		if (i == max + 10)
			pc_unresolved("%ld signals were queued without EAGAIN ({SIGQUEUE_MAX} %ld)", i, max);
		pc_check(errno == EAGAIN, "sigqueue() failed with %s, expected EAGAIN", pc_errname(errno));
		while (sigtimedwait(&one, &info, &zero) == SIGRTMIN)
			taken++;
		pc_check(taken == i, "%ld instances were accepted, %ld queued", taken, i);
		pc_check(sigqueue(getpid(), SIGRTMIN, v) == 0, "sigqueue() after the queue was emptied failed: %s",
			 pc_errname(errno));
		(void)sigtimedwait(&one, &info, &zero);
	}
	pc_pass();
}
