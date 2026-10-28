/*
 * pcth.h - helpers shared by the tests of XSH 2.9 (threads).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Flags shared between threads live in a small array guarded by a mutex
 * that no test ever waits on while holding it, so that nothing under test
 * can interfere with the bookkeeping.
 */
#ifndef PCTH_H
#define PCTH_H

#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static pthread_mutex_t pcth_lock = PTHREAD_MUTEX_INITIALIZER;
static int pcth_flags[32];

static inline void flag_set(int i, int v)
{
	(void)pthread_mutex_lock(&pcth_lock);
	pcth_flags[i] = v;
	(void)pthread_mutex_unlock(&pcth_lock);
}

static inline int flag_get(int i)
{
	int v;

	(void)pthread_mutex_lock(&pcth_lock);
	v = pcth_flags[i];
	(void)pthread_mutex_unlock(&pcth_lock);
	return v;
}

static inline int flag_add(int i, int d)
{
	int v;

	(void)pthread_mutex_lock(&pcth_lock);
	pcth_flags[i] += d;
	v = pcth_flags[i];
	(void)pthread_mutex_unlock(&pcth_lock);
	return v;
}

static inline long now_ms(void)
{
	struct timespec ts;

	(void)clock_gettime(CLOCK_MONOTONIC, &ts);
	return (long)ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
}

static inline void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Waits up to ms milliseconds for flag i to be at least v: 1 if it is. */
static inline int wait_flag(int i, int v, long ms)
{
	long end = now_ms() + ms;

	while (flag_get(i) < v) {
		if (now_ms() >= end)
			return 0;
		sleep_ms(1);
	}
	return 1;
}

/* The time on clock c, ms milliseconds from now. */
static inline struct timespec ts_after(clockid_t c, long ms)
{
	struct timespec ts;

	(void)clock_gettime(c, &ts);
	ts.tv_sec += ms / 1000;
	ts.tv_nsec += (ms % 1000) * 1000000L;
	if (ts.tv_nsec >= 1000000000L) {
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000L;
	}
	return ts;
}

/*
 * Joins t through a joiner thread, waiting at most ms milliseconds, so
 * that a thread that never ends gives a verdict instead of a hang: 1 and
 * the thread's status in *ret if it was joined in time, 0 if not.
 * Uses flag slot 31.
 */
static pthread_t pcth_join_target;
static void *pcth_join_status;

static void *pcth_joiner(void *arg)
{
	(void)arg;
	if (pthread_join(pcth_join_target, &pcth_join_status) == 0)
		flag_set(31, 1);
	else
		flag_set(31, 2);
	return NULL;
}

static inline int join_within(pthread_t t, long ms, void **ret)
{
	pthread_t j;

	flag_set(31, 0);
	pcth_join_target = t;
	if (pthread_create(&j, NULL, pcth_joiner, NULL) != 0)
		pc_unresolved("pthread_create (joiner) failed");
	if (!wait_flag(31, 1, ms))
		return 0;
	(void)pthread_join(j, NULL);
	if (flag_get(31) != 1)
		pc_unresolved("pthread_join failed");
	if (ret != NULL)
		*ret = pcth_join_status;
	return 1;
}

/* Starts fn(arg) in a new thread with default attributes. */
static inline pthread_t start(void *(*fn)(void *), void *arg)
{
	pthread_t t;

	if (pthread_create(&t, NULL, fn, arg) != 0)
		pc_unresolved("pthread_create failed");
	return t;
}

/* A call that blocks must not hang the test: the last resort is UNRESOLVED. */
static void pcth_on_guard(int sig)
{
	(void)sig;
	pc_unresolved("a call did not finish within the guard time");
}

static inline void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = pcth_on_guard;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(secs);
}

#endif
