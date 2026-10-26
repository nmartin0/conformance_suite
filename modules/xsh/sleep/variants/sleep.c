/*
 * sleep.c - conforming variants of sleep(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "pcvariant.h"

#define SEC 1000000000LL

/* Whether the process has made a second thread (then SIGALRM is off limits). */
static volatile int threaded;

int pthread_create(pthread_t *t, const pthread_attr_t *a, void *(*fn)(void *), void *arg)
{
	REAL(pthread_create);

	threaded = 1;
	return real_pthread_create(t, a, fn, arg);
}

static long long mono(void)
{
	struct timespec t;

	(void)clock_gettime(CLOCK_MONOTONIC, &t);
	return (long long)t.tv_sec * SEC + t.tv_nsec;
}

/* Sleeps ns on CLOCK_MONOTONIC; a caught signal ends it. Returns the ns left (0 when done). */
static long long nap(long long ns)
{
	struct timespec t, rem;

	if (ns <= 0)
		return 0;
	t.tv_sec = (time_t)(ns / SEC);
	t.tv_nsec = (long)(ns % SEC);
	if (clock_nanosleep(CLOCK_MONOTONIC, 0, &t, &rem) == EINTR)
		return (long long)rem.tv_sec * SEC + rem.tv_nsec;
	return 0;
}

static volatile sig_atomic_t rang;

static void ring(int sig)
{
	(void)sig;
	rang = 1;
}

/*
 * "via_alarm": in a single-threaded process, sleep() is built on SIGALRM,
 * as sleep() DESCRIPTION allows there (and its RATIONALE describes): with
 * SIGALRM blocked, its action saved and a private handler installed, it
 * calls alarm() and waits in sigsuspend() with the caller's mask (SIGALRM
 * unblocked); afterwards it puts the action and the mask back and returns
 * what alarm(0) says is left (0 when the alarm went off). An alarm the
 * caller had already set is left alone: the real sleep() is used then,
 * and in any process with a second thread, where SIGALRM may not be used.
 *
 * "late": a sleep that ran its full time returns a second late (scheduling
 * of other activity, DESCRIPTION; no upper bound is given).
 *
 * "early_half": the sleep ends half a second before the time asked for.
 * Allowed only under the reading of Q-14 that sleep(), like alarm(), may
 * return up to a second early (RATIONALE); the disputed XSH.sleep.002
 * declares it as its READING, and the required tests must hold either
 * way.
 *
 * "round_nearest": an interrupted sleep() returns the unslept time rounded
 * to the nearest second (at least 1). RETURN VALUE asks for the unslept
 * amount in seconds and does not say how a fraction is rounded.
 */
unsigned sleep(unsigned sec)
{
	const char *v = variant_of("sleep.");
	long long left;
	REAL(sleep);

	if (v == NULL)
		return real_sleep(sec);
	if (IS("via_alarm") && !threaded && sec > 0) {
		struct sigaction sa, old;
		sigset_t block, saved, wait;
		unsigned prev, r;

		prev = alarm(0);
		if (prev != 0) {
			(void)alarm(prev);
			return real_sleep(sec);
		}
		sigemptyset(&block);
		sigaddset(&block, SIGALRM);
		(void)sigprocmask(SIG_BLOCK, &block, &saved);
		memset(&sa, 0, sizeof sa);
		sa.sa_handler = ring;
		sigemptyset(&sa.sa_mask);
		(void)sigaction(SIGALRM, &sa, &old);
		rang = 0;
		(void)alarm(sec);
		wait = saved;
		sigdelset(&wait, SIGALRM);
		(void)sigsuspend(&wait);
		r = rang ? 0 : alarm(0);
		(void)alarm(0);
		(void)sigaction(SIGALRM, &old, NULL);
		(void)sigprocmask(SIG_SETMASK, &saved, NULL);
		return r;
	}
	if (IS("late")) {
		unsigned r = real_sleep(sec);

		if (r == 0) {
			long long end = mono() + SEC;

			while (mono() < end)
				(void)nap(end - mono());
		}
		return r;
	}
	if (IS("early_half") && sec > 0) {
		left = nap((long long)sec * SEC - SEC / 2);
		return left == 0 ? 0 : (unsigned)((left + SEC / 2 + SEC - 1) / SEC);
	}
	if (IS("round_nearest")) {
		left = nap((long long)sec * SEC);
		if (left == 0)
			return 0;
		left = (left + SEC / 2) / SEC;
		return left < 1 ? 1 : (unsigned)left;
	}
	return real_sleep(sec);
}
