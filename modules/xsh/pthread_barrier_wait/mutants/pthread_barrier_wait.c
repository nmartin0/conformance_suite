/*
 * pthread_barrier_wait.c - planted faults in pthread_barrier_wait() (and
 * in the count pthread_barrier_init() records), for mutation checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_barrier_wait.<fault>" and
 * apply while PC_PHASE is "test". Each barrier's count is recorded at
 * pthread_barrier_init() in every phase; the wait_eintr fault replaces
 * the barrier with one of its own that polls.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static char phase[16];

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (strcmp(name, "PC_PHASE") == 0 && strlen(value) < sizeof phase)
		strcpy(phase, value);
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");
	size_t n = strlen(prefix);

	if (strcmp(phase, "test") != 0 || m == NULL || strncmp(m, prefix, n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

struct rec {
	void *b;
	unsigned count, arrived, gen;
	int completed, serial_given;
};

#define NREC 16
static struct rec recs[NREC];
static atomic_flag guard = ATOMIC_FLAG_INIT;

static void take(void) { while (atomic_flag_test_and_set(&guard)) ; }
static void give(void) { atomic_flag_clear(&guard); }

/* Call under guard. */
static struct rec *rec_of(void *b)
{
	int i;

	for (i = 0; i < NREC; i++)
		if (recs[i].b == b)
			return &recs[i];
	for (i = 0; i < NREC; i++)
		if (recs[i].b == NULL) {
			memset(&recs[i], 0, sizeof recs[i]);
			recs[i].b = b;
			return &recs[i];
		}
	return &recs[0];
}

int pthread_barrier_init(pthread_barrier_t *restrict b, const pthread_barrierattr_t *restrict a, unsigned count)
{
	static int (*real)(pthread_barrier_t *restrict, const pthread_barrierattr_t *restrict, unsigned);
	const char *m = mutant_of("pthread_barrier_wait.");
	struct rec *r;

	if (real == NULL)
		real = (int (*)(pthread_barrier_t *restrict, const pthread_barrierattr_t *restrict, unsigned))
		    dlsym(RTLD_NEXT, "pthread_barrier_init");
	take();
	r = rec_of(b);
	memset(r, 0, sizeof *r);
	r->b = b;
	r->count = count;
	give();
	if (IS("count_minus_one") && count > 1)
		count--;
	return real(b, a, count);
}

/* A polling barrier of the library's own that gives up on a signal. */
static int interruptible_wait(void *b)
{
	struct rec *r;
	unsigned gen0;

	take();
	r = rec_of(b);
	gen0 = r->gen;
	if (++r->arrived >= r->count) {
		r->arrived = 0;
		r->gen++;
		give();
		return PTHREAD_BARRIER_SERIAL_THREAD;
	}
	give();
	for (;;) {
		struct timespec ts = { 0, 2000000L };
		int intr = nanosleep(&ts, NULL) == -1 && errno == EINTR;

		take();
		if (r->gen != gen0) {
			give();
			return 0;
		}
		if (intr) {
			r->arrived--;
			give();
			return EINTR;
		}
		give();
	}
}

int pthread_barrier_wait(pthread_barrier_t *b)
{
	static int (*real)(pthread_barrier_t *);
	const char *m = mutant_of("pthread_barrier_wait.");
	struct rec *rc;
	int r, done;

	if (real == NULL)
		real = (int (*)(pthread_barrier_t *))dlsym(RTLD_NEXT, "pthread_barrier_wait");
	if (IS("no_wait"))
		return 0;
	if (IS("wait_eintr"))
		return interruptible_wait(b);
	take();
	done = rec_of(b)->completed;
	give();
	if (IS("no_reset") && done)
		return 0;
	r = real(b);
	take();
	rc = rec_of(b);
	rc->completed = 1;
	if (r == PTHREAD_BARRIER_SERIAL_THREAD) {
		if (IS("serial_once") && rc->serial_given)
			r = 0;
		rc->serial_given = 1;
	}
	give();
	if (r == PTHREAD_BARRIER_SERIAL_THREAD && IS("serial_none"))
		return 0;
	if (r == 0 && IS("serial_all"))
		return PTHREAD_BARRIER_SERIAL_THREAD;
	return r;
}
