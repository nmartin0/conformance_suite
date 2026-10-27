/*
 * A barrier keeps working after the attributes object it was made from is destroyed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_barrierattr_init.003
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static pthread_barrier_t b;
static atomic_int done[2], result[2];

static void *helper(void *arg)
{
	int k = *(int *)arg;

	atomic_store(&result[k], pthread_barrier_wait(&b));
	atomic_store(&done[k], 1);
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
	pthread_barrierattr_t a;
	pthread_t t[2];
	int id[2] = { 0, 1 };
	int r, k, i;

	pc_begin("XSH.pthread_barrierattr_init.003");
	NEED(pthread_barrierattr_init(&a) == 0, "pthread_barrierattr_init failed");
	NEED(pthread_barrier_init(&b, &a, 2) == 0, "pthread_barrier_init failed");
	pc_test();
	r = pthread_barrierattr_destroy(&a);
	pc_check(r == 0, "pthread_barrierattr_destroy() returned %s", rn(r));
	for (k = 0; k < 2; k++)
		NEED(pthread_create(&t[k], NULL, helper, &id[k]) == 0, "pthread_create failed");
	for (i = 0; i < 2000 && !(atomic_load(&done[0]) && atomic_load(&done[1])); i++)
		sleep_ms(5);
	pc_check(atomic_load(&done[0]) && atomic_load(&done[1]), "the two threads did not both leave the barrier within 10 s");
	for (k = 0; k < 2; k++)
		NEED(pthread_join(t[k], NULL) == 0, "pthread_join failed");
	pc_check((atomic_load(&result[0]) == PTHREAD_BARRIER_SERIAL_THREAD && atomic_load(&result[1]) == 0) ||
	    (atomic_load(&result[0]) == 0 && atomic_load(&result[1]) == PTHREAD_BARRIER_SERIAL_THREAD),
	    "pthread_barrier_wait() returned %s and %s, expected one PTHREAD_BARRIER_SERIAL_THREAD and one 0",
	    rn(atomic_load(&result[0])), rn(atomic_load(&result[1])));
	(void)pthread_barrier_destroy(&b);
	pc_pass();
}
