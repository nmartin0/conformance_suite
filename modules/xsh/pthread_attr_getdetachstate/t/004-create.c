/*
 * Threads created with a joinable or detached attributes object.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getdetachstate.004
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static atomic_int runs;
static int token;

static void *routine(void *arg)
{
	atomic_fetch_add(&runs, 1);
	return arg;
}

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	struct timespec ts = { 0, 5000000L };
	pthread_attr_t a;
	pthread_t t;
	void *out = NULL;
	int r, i;

	pc_begin("XSH.pthread_attr_getdetachstate.004");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	NEED(pthread_attr_setdetachstate(&a, PTHREAD_CREATE_JOINABLE) == 0, "setdetachstate(JOINABLE) failed");
	pc_test();
	r = pthread_create(&t, &a, routine, &token);
	if (r == EAGAIN)
		pc_unresolved("pthread_create() failed with EAGAIN");
	pc_check(r == 0, "pthread_create() with a joinable attributes object returned %s", rn(r));
	r = pthread_join(t, &out);
	pc_check(r == 0, "pthread_join() of the joinable thread returned %s", rn(r));
	pc_check(out == &token, "pthread_join() gave %p, expected the routine's return value %p", out, (void *)&token);
	pc_check(atomic_load(&runs) == 1, "the routine ran %d times, expected 1", atomic_load(&runs));

	NEED(pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED) == 0, "setdetachstate(DETACHED) failed");
	r = pthread_create(&t, &a, routine, &token);
	if (r == EAGAIN)
		pc_unresolved("pthread_create() failed with EAGAIN");
	pc_check(r == 0, "pthread_create() with a detached attributes object returned %s", rn(r));
	for (i = 0; i < 2000 && atomic_load(&runs) < 2; i++)
		(void)nanosleep(&ts, NULL);
	pc_check(atomic_load(&runs) == 2, "the detached thread's routine ran %d times in all, expected 2",
	    atomic_load(&runs));
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
