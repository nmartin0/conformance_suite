/*
 * errno is thread-local.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.errno.003
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>

#define ROUNDS 100000

static atomic_int go, other_done, other_bad;

static void *other(void *arg)
{
	int i, bad = 0;

	(void)arg;
	while (!atomic_load(&go))
		;
	for (i = 0; i < ROUNDS; i++) {
		errno = EDOM;
		if (errno != EDOM)
			bad++;
	}
	atomic_store(&other_bad, bad);
	atomic_store(&other_done, 1);
	return NULL;
}

int main(void)
{
	pthread_t t;
	int i, bad = 0, rc;
	struct timespec ts;

	pc_begin("XSH.errno.003");
	errno = ERANGE;
	rc = pthread_create(&t, NULL, other, NULL);
	if (rc != 0)
		pc_unresolved("pthread_create failed: %d", rc);
	pc_test();
	atomic_store(&go, 1);
	for (i = 0; i < ROUNDS; i++) {
		errno = ERANGE;
		if (errno != ERANGE)
			bad++;
	}
	ts.tv_sec = 0;
	ts.tv_nsec = 1000000;
	for (i = 0; i < 5000 && !atomic_load(&other_done); i++)
		(void)nanosleep(&ts, NULL);
	pc_check(atomic_load(&other_done), "the other thread did not finish");
	(void)pthread_join(t, NULL);
	pc_check(bad == 0, "the main thread's errno changed %d times under its feet", bad);
	pc_check(atomic_load(&other_bad) == 0, "the other thread's errno changed %d times under its feet", atomic_load(&other_bad));
	pc_check(errno == ERANGE, "the main thread's errno is %s after the join, expected ERANGE", pc_errname(errno));
	pc_pass();
}
