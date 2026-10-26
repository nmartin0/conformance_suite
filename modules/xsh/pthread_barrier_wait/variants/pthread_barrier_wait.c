/*
 * pthread_barrier_wait.c - conforming variants of pthread_barrier_wait(),
 * for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>
#include "pcvariant.h"

/*
 * "serial_first": the barrier is the library's own (it polls), and
 * PTHREAD_BARRIER_SERIAL_THREAD goes to the first thread to arrive in
 * each round, not the last. The page gives the constant to "one
 * unspecified thread". A signal does not end the wait, as the page
 * requires.
 */
struct rec {
	void *b;
	unsigned count, arrived, gen;
};

#define NREC 16
static struct rec recs[NREC];
static atomic_flag guard = ATOMIC_FLAG_INIT;

static void take(void) { while (atomic_flag_test_and_set(&guard)) ; }
static void give(void) { atomic_flag_clear(&guard); }

static struct rec *rec_of(void *b)
{
	int i;

	for (i = 0; i < NREC; i++)
		if (recs[i].b == b)
			return &recs[i];
	for (i = 0; i < NREC; i++)
		if (recs[i].b == NULL) {
			recs[i].b = b;
			return &recs[i];
		}
	return &recs[0];
}

int pthread_barrier_init(pthread_barrier_t *restrict b, const pthread_barrierattr_t *restrict a, unsigned count)
{
	const char *v = variant_of("pthread_barrier_wait.");
	struct rec *r;
	REAL(pthread_barrier_init);

	if (IS("serial_first") && count > 0) {
		take();
		r = rec_of(b);
		r->count = count;
		r->arrived = 0;
		give();
	}
	return real_pthread_barrier_init(b, a, count);
}

int pthread_barrier_wait(pthread_barrier_t *b)
{
	const char *v = variant_of("pthread_barrier_wait.");
	struct rec *r;
	unsigned gen0;
	int first;
	REAL(pthread_barrier_wait);

	if (!IS("serial_first"))
		return real_pthread_barrier_wait(b);
	take();
	r = rec_of(b);
	gen0 = r->gen;
	first = ++r->arrived == 1;
	if (r->arrived >= r->count) {
		r->arrived = 0;
		r->gen++;
		give();
		return first ? PTHREAD_BARRIER_SERIAL_THREAD : 0;
	}
	give();
	for (;;) {
		struct timespec ts = { 0, 2000000L };

		(void)nanosleep(&ts, NULL);
		take();
		if (r->gen != gen0) {
			give();
			return first ? PTHREAD_BARRIER_SERIAL_THREAD : 0;
		}
		give();
	}
}
