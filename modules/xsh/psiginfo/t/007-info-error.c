/*
 * On a write error psiginfo() sets the error indicator and errno.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.psiginfo.007
 * MUTANT    psiginfo.error_hidden
 * VARIANT   psiginfo.own_text
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

static siginfo_t keep;
static volatile sig_atomic_t got;

static void h(int s, siginfo_t *i, void *u)
{
	(void)s;
	(void)u;
	keep = *i;
	got = 1;
}

int main(void)
{
	pc_begin("XSH.psiginfo.007");
	{
		struct sigaction sa;
		int saved, e, fe;

		memset(&sa, 0, sizeof sa);
		sa.sa_sigaction = h;
		sa.sa_flags = SA_SIGINFO;
		sigemptyset(&sa.sa_mask);
		pc_check(sigaction(SIGUSR1, &sa, NULL) == 0, "sigaction failed");
		pc_check(raise(SIGUSR1) == 0 && got, "SIGUSR1 was not caught");
		saved = dup(2);
		pc_check(saved != -1, "dup failed");
		clearerr(stderr);
		pc_test();
		(void)close(2);
		errno = 0;
		psiginfo(&keep, "pc");
		e = errno;
		fe = ferror(stderr);
		(void)dup2(saved, 2);
		clearerr(stderr);
		pc_check(fe != 0, "the error indicator of stderr is not set");
		pc_check(e != 0, "errno was not set");
	}
	pc_pass();
}
