/*
 * A CLOCK_MONOTONIC condition variable measures pthread_cond_timedwait() on that clock.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_condattr_getclock.006
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

struct waiter {
	pthread_cond_t *c;
	pthread_mutex_t *m;
	struct timespec abs, end;
	atomic_int done;
	int r;
};

static void *waiter_main(void *p)
{
	struct waiter *w = p;

	(void)pthread_mutex_lock(w->m);
	do	/* nobody signals: a 0 return is a spurious wakeup, which the page allows */
		w->r = pthread_cond_timedwait(w->c, w->m, &w->abs);
	while (w->r == 0);
	(void)clock_gettime(CLOCK_MONOTONIC, &w->end);
	(void)pthread_mutex_unlock(w->m);
	atomic_store(&w->done, 1);
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

static int before(const struct timespec *a, const struct timespec *b)
{
	return a->tv_sec < b->tv_sec || (a->tv_sec == b->tv_sec && a->tv_nsec < b->tv_nsec);
}

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
	pthread_condattr_t a;
	pthread_cond_t c;
	pthread_t t;
	struct waiter w;
	int i;

	pc_begin("XSH.pthread_condattr_getclock.006");
	NEED(pthread_condattr_init(&a) == 0, "pthread_condattr_init failed");
	NEED(pthread_condattr_setclock(&a, CLOCK_MONOTONIC) == 0, "pthread_condattr_setclock(CLOCK_MONOTONIC) failed");
	NEED(pthread_cond_init(&c, &a) == 0, "pthread_cond_init failed");
	NEED(pthread_condattr_destroy(&a) == 0, "pthread_condattr_destroy failed");
	NEED(clock_gettime(CLOCK_MONOTONIC, &w.abs) == 0, "clock_gettime failed");
	w.abs.tv_nsec += 150000000L;
	if (w.abs.tv_nsec >= 1000000000L) {
		w.abs.tv_sec++;
		w.abs.tv_nsec -= 1000000000L;
	}
	w.c = &c;
	w.m = &m;
	w.r = -1;
	atomic_init(&w.done, 0);
	pc_test();
	NEED(pthread_create(&t, NULL, waiter_main, &w) == 0, "pthread_create failed");
	for (i = 0; i < 2000 && !atomic_load(&w.done); i++)
		sleep_ms(5);
	pc_check(atomic_load(&w.done), "the wait for a time 150 ms ahead on CLOCK_MONOTONIC did not return within 10 s");
	NEED(pthread_join(t, NULL) == 0, "pthread_join failed");
	pc_check(w.r == ETIMEDOUT, "the wait returned %s, expected ETIMEDOUT", rn(w.r));
	pc_check(!before(&w.end, &w.abs), "the wait timed out before the CLOCK_MONOTONIC time had passed");
	(void)pthread_cond_destroy(&c);
	pc_pass();
}
