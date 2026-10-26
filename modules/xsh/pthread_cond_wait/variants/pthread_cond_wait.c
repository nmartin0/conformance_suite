/*
 * pthread_cond_wait.c - conforming variants of the condition waits, for
 * the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 *
 * "spurious": every wait returns 0 after at most 20 ms even when nobody
 * signaled. The pthread_cond_wait() page lets any of the three waits
 * wake spuriously and says a return implies nothing about the predicate;
 * a timed wait still returns ETIMEDOUT once abstime has passed, and every
 * error check, the mutex hand-over and cancellation are the system's own
 * (the short waits are real timed waits on the same condition variable).
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include "pcvariant.h"

#define SLICE_NS 20000000L

typedef int (*clock_fn)(pthread_cond_t *, pthread_mutex_t *, clockid_t, const struct timespec *);

/* The clock attribute of each condition variable initialized with one. */
static pthread_mutex_t tab_lock = PTHREAD_MUTEX_INITIALIZER;
static const void *tab_key[64];
static clockid_t tab_clock[64];

static void remember(const void *key, clockid_t k)
{
	int i;

	(void)pthread_mutex_lock(&tab_lock);
	for (i = 0; i < 64; i++)
		if (tab_key[i] == key || tab_key[i] == NULL) {
			tab_key[i] = key;
			tab_clock[i] = k;
			break;
		}
	(void)pthread_mutex_unlock(&tab_lock);
}

static clockid_t clock_of(const void *key)
{
	clockid_t k = CLOCK_REALTIME;	/* the default clock attribute */
	int i;

	(void)pthread_mutex_lock(&tab_lock);
	for (i = 0; i < 64 && tab_key[i] != NULL; i++)
		if (tab_key[i] == key) {
			k = tab_clock[i];
			break;
		}
	(void)pthread_mutex_unlock(&tab_lock);
	return k;
}

int pthread_condattr_setclock(pthread_condattr_t *a, clockid_t k)
{
	REAL(pthread_condattr_setclock);
	int r = real_pthread_condattr_setclock(a, k);

	if (r == 0)
		remember(a, k);
	return r;
}

int pthread_cond_init(pthread_cond_t *restrict cv, const pthread_condattr_t *restrict a)
{
	REAL(pthread_cond_init);
	int r = real_pthread_cond_init(cv, a);

	if (r == 0)
		remember(cv, a != NULL ? clock_of(a) : CLOCK_REALTIME);
	return r;
}

/* The earlier of abstime and now + SLICE_NS on clock k; 1 if the slice is earlier. */
static int shorter(clockid_t k, const struct timespec *abstime, struct timespec *out)
{
	struct timespec now;

	(void)clock_gettime(k, &now);
	now.tv_nsec += SLICE_NS;
	if (now.tv_nsec >= 1000000000L) {
		now.tv_sec++;
		now.tv_nsec -= 1000000000L;
	}
	if (abstime != NULL && (abstime->tv_sec < now.tv_sec ||
				(abstime->tv_sec == now.tv_sec && abstime->tv_nsec <= now.tv_nsec))) {
		*out = *abstime;
		return 0;
	}
	*out = now;
	return 1;
}

static int bad_nsec(const struct timespec *t)
{
	return t->tv_nsec < 0 || t->tv_nsec >= 1000000000L;
}

int pthread_cond_wait(pthread_cond_t *restrict cv, pthread_mutex_t *restrict mx)
{
	const char *v = variant_of("pthread_cond_wait.");
	struct timespec t;
	int r;
	REAL(pthread_cond_wait);
	REAL(pthread_cond_timedwait);

	if (!IS("spurious"))
		return real_pthread_cond_wait(cv, mx);
	(void)shorter(clock_of(cv), NULL, &t);
	r = real_pthread_cond_timedwait(cv, mx, &t);
	return r == ETIMEDOUT ? 0 : r;
}

int pthread_cond_timedwait(pthread_cond_t *restrict cv, pthread_mutex_t *restrict mx,
			   const struct timespec *restrict abstime)
{
	const char *v = variant_of("pthread_cond_wait.");
	struct timespec t;
	int r;
	REAL(pthread_cond_timedwait);

	if (!IS("spurious") || bad_nsec(abstime) || !shorter(clock_of(cv), abstime, &t))
		return real_pthread_cond_timedwait(cv, mx, abstime);
	r = real_pthread_cond_timedwait(cv, mx, &t);
	return r == ETIMEDOUT ? 0 : r;
}

int pthread_cond_clockwait(pthread_cond_t *restrict cv, pthread_mutex_t *restrict mx, clockid_t k,
			   const struct timespec *restrict abstime)
{
	const char *v = variant_of("pthread_cond_wait.");
	static clock_fn real;
	struct timespec t;
	int r;

	if (real == NULL)
		real = (clock_fn)dlsym(RTLD_NEXT, "pthread_cond_clockwait");
	if (real == NULL)
		return ENOSYS;
	if (!IS("spurious") || bad_nsec(abstime) || (k != CLOCK_REALTIME && k != CLOCK_MONOTONIC) ||
	    !shorter(k, abstime, &t))
		return real(cv, mx, k, abstime);
	r = real(cv, mx, k, &t);
	return r == ETIMEDOUT ? 0 : r;
}
