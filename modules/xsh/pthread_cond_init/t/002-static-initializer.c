/*
 * PTHREAD_COND_INITIALIZER gives a working condition variable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_cond_init.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t c = PTHREAD_COND_INITIALIZER;
static int waiting, pred;	/* protected by m */
static atomic_int done, result = -1;

static void *waiter(void *arg)
{
	struct timespec ts;
	int r = 0;

	(void)arg;
	(void)pthread_mutex_lock(&m);
	waiting = 1;
	(void)clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += 10;
	while (!pred && r == 0)
		r = pthread_cond_timedwait(&c, &m, &ts);
	atomic_store(&result, pred ? 0 : r);
	(void)pthread_mutex_unlock(&m);
	atomic_store(&done, 1);
	return NULL;
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	pthread_t t;
	int r, i, w = 0;

	pc_begin("XSH.pthread_cond_init.002");
	pc_test();
	NEED(pthread_create(&t, NULL, waiter, NULL) == 0, "pthread_create failed");
	for (i = 0; i < 2000 && !w; i++) {
		NEED(pthread_mutex_lock(&m) == 0, "pthread_mutex_lock failed");
		w = waiting;
		if (w) {
			pred = 1;
			r = pthread_cond_signal(&c);
			pc_check(r == 0, "pthread_cond_signal() returned %s", rn(r));
		}
		(void)pthread_mutex_unlock(&m);
		if (!w)
			sleep_ms(5);
	}
	NEED(w, "the waiter did not start");
	for (i = 0; i < 2000 && !atomic_load(&done); i++)
		sleep_ms(5);
	pc_check(atomic_load(&done), "the waiting thread was not woken within 10 s");
	NEED(pthread_join(t, NULL) == 0, "pthread_join failed");
	pc_check(atomic_load(&result) == 0, "the wait ended with %s instead of the predicate becoming true", rn(atomic_load(&result)));
	pc_pass();
}
