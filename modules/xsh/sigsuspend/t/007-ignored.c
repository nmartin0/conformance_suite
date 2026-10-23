/*
 * An ignored signal does not end sigsuspend().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigsuspend.007
 * MUTANT    sigsuspend.ignored_wakes
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

static volatile sig_atomic_t got2;

static void h2(int s)
{
	(void)s;
	got2 = 1;
}

int main(void)
{
	pc_begin("XSH.sigsuspend.007");
	{
		struct sigaction sa;
		sigset_t one, none;
		pid_t kid, me = getpid();
		int st, r, e;

		(void)signal(SIGUSR1, SIG_IGN);
		memset(&sa, 0, sizeof sa);
		sa.sa_handler = h2;
		sigemptyset(&sa.sa_mask);
		pc_check(sigaction(SIGUSR2, &sa, NULL) == 0, "sigaction failed");
		sigemptyset(&one);
		sigaddset(&one, SIGUSR2);
		pc_check(sigprocmask(SIG_BLOCK, &one, NULL) == 0, "sigprocmask failed");
		sigemptyset(&none);
		kid = fork();
		if (kid == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (kid == 0) {
			int i;
			for (i = 0; i < 4; i++) {
				sleep_ms(50);
				(void)kill(me, SIGUSR1);
			}
			sleep_ms(300);
			(void)kill(me, SIGUSR2);
			_exit(0);
		}
		pc_test();
		r = sigsuspend(&none);
		e = errno;
		pc_check(got2, "sigsuspend() returned before SIGUSR2 arrived (an ignored SIGUSR1 ended it)");
		pc_check(r == -1 && e == EINTR, "sigsuspend() returned %d (%s), expected -1 with EINTR", r, pc_errname(e));
		(void)waitpid(kid, &st, 0);
	}
	pc_pass();
}
