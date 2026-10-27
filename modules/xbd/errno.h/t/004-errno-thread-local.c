/*
 * A write to errno in one thread does not show in another thread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.errno.h.004
 */
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include "pc.h"

static atomic_int go, done;

/* Spins (no library calls, so errno is left alone) until f is set; 0 if it never is. */
static int spin(atomic_int *f)
{
	long i;

	for (i = 0; i < 400000000L; i++)
		if (atomic_load(f))
			return 1;
	return 0;
}

static void *other(void *arg)
{
	(void)arg;
	if (!spin(&go))
		return (void *)1;
	errno = 22222;
	atomic_store(&done, 1);
	return NULL;
}

int main(void)
{
	pthread_t t;
	void *r;
	int rc, ok, e;

	pc_begin("XBD.errno.h.004");
	rc = pthread_create(&t, NULL, other, NULL);
	if (rc != 0)
		pc_unresolved("pthread_create failed: %d", rc);
	pc_test();
	errno = 11111;
	atomic_store(&go, 1);
	ok = spin(&done);
	e = errno;
	if (!ok) {
		atomic_store(&done, 1);
		pthread_join(t, &r);
		pc_unresolved("the other thread did not run in time");
	}
	pthread_join(t, &r);
	pc_check(e == 11111, "errno was %d after another thread stored 22222 in its own errno; it is not thread local", e);
	pc_pass();
}
