/*
 * pthread_barrier_init() can be called again after pthread_barrier_destroy(), with another count.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_barrier_init.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)
#define N 3

static pthread_barrier_t b;
static atomic_int done[N], result[N];

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
	pthread_t t[N];
	int id[N], r, k, i, serial = 0, plain = 0;

	pc_begin("XSH.pthread_barrier_init.002");
	NEED(pthread_barrier_init(&b, NULL, 2) == 0, "pthread_barrier_init failed");
	NEED(pthread_barrier_destroy(&b) == 0, "pthread_barrier_destroy failed");
	pc_test();
	r = pthread_barrier_init(&b, NULL, N);
	pc_check(r == 0, "pthread_barrier_init(NULL attributes, %d) after a destroy returned %s", N, rn(r));
	for (k = 0; k < N; k++) {
		id[k] = k;
		NEED(pthread_create(&t[k], NULL, helper, &id[k]) == 0, "pthread_create failed");
	}
	for (i = 0; i < 2000; i++) {
		int all = 1;

		for (k = 0; k < N; k++)
			if (!atomic_load(&done[k]))
				all = 0;
		if (all)
			break;
		sleep_ms(5);
	}
	for (k = 0; k < N; k++)
		pc_check(atomic_load(&done[k]), "thread %d did not leave the barrier within 10 s", k);
	for (k = 0; k < N; k++) {
		NEED(pthread_join(t[k], NULL) == 0, "pthread_join failed");
		if (atomic_load(&result[k]) == PTHREAD_BARRIER_SERIAL_THREAD)
			serial++;
		else if (atomic_load(&result[k]) == 0)
			plain++;
	}
	pc_check(serial == 1 && plain == N - 1, "%d threads got PTHREAD_BARRIER_SERIAL_THREAD and %d got 0, expected 1 and %d", serial,
	    plain, N - 1);
	r = pthread_barrier_destroy(&b);
	pc_check(r == 0, "pthread_barrier_destroy() returned %s", rn(r));
	pc_pass();
}
