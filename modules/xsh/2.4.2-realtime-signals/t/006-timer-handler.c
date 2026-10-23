/*
 * A timer's sigev_value reaches a SA_SIGINFO handler.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.4.2-realtime-signals.006
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

static volatile sig_atomic_t got;
static volatile int code, val;

static void h(int s, siginfo_t *i, void *u)
{
	(void)s;
	(void)u;
	code = i->si_code;
	val = i->si_value.sival_int;
	got = 1;
}

int main(void)
{
	pc_begin("XSH.2.4.2-realtime-signals.006");
	{
		struct sigaction sa;
		struct sigevent ev;
		struct itimerspec it;
		timer_t t;

		memset(&sa, 0, sizeof sa);
		sa.sa_sigaction = h;
		sa.sa_flags = SA_SIGINFO;
		sigemptyset(&sa.sa_mask);
		pc_check(sigaction(SIGRTMIN, &sa, NULL) == 0, "sigaction failed");
		memset(&ev, 0, sizeof ev);
		ev.sigev_notify = SIGEV_SIGNAL;
		ev.sigev_signo = SIGRTMIN;
		ev.sigev_value.sival_int = 4242;
		memset(&it, 0, sizeof it);
		it.it_value.tv_nsec = 10000000;
		pc_test();
		if (timer_create(CLOCK_REALTIME, &ev, &t) != 0) {
			if (errno == EAGAIN)
				pc_unresolved("timer_create: no timer available (EAGAIN)");
			pc_fail("timer_create failed: %s", pc_errname(errno));
		}
		pc_check(timer_settime(t, 0, &it, NULL) == 0, "timer_settime failed: %s", pc_errname(errno));
		pc_check(wait_for(&got, 3000), "the handler did not run");
		pc_check(code == SI_TIMER, "si_code is %d, expected SI_TIMER (%d)", code, SI_TIMER);
		pc_check(val == 4242, "si_value is %d, expected 4242", val);
	}
	pc_pass();
}
