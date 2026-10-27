/*
 * PTHREAD_NULL initialises a static pthread_t and an automatic one, and
 * compares equal to neither the calling thread's ID nor that of a running
 * thread created by the test.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.012
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

#ifdef PTHREAD_NULL
static pthread_t none = PTHREAD_NULL;
#endif
static pthread_mutex_t gate = PTHREAD_MUTEX_INITIALIZER;

static void *body(void *arg)
{
	(void)pthread_mutex_lock(&gate);
	(void)pthread_mutex_unlock(&gate);
	return arg;
}

int main(void)
{
	pc_begin("XBD.pthread.h.012");
#ifndef PTHREAD_NULL
	pc_unresolved("PTHREAD_NULL is missing (the header check reports it)");
#else
	pthread_t t, z = PTHREAD_NULL;
	int e;

	pc_test();
	pc_check(pthread_equal(z, none), "two objects initialised with PTHREAD_NULL differ");
	pc_check(!pthread_equal(none, pthread_self()), "PTHREAD_NULL equals the thread ID of the calling thread");
	/* The new thread waits for the gate, so it is certainly alive while it is compared. */
	e = pthread_mutex_lock(&gate);
	if (e != 0)
		pc_unresolved("pthread_mutex_lock failed (%s)", pc_errname(e));
	e = pthread_create(&t, NULL, body, NULL);
	if (e != 0)
		pc_unresolved("pthread_create failed (%s)", pc_errname(e));
	pc_check(!pthread_equal(none, t), "PTHREAD_NULL equals the thread ID of a running thread");
	(void)pthread_mutex_unlock(&gate);
	e = pthread_join(t, NULL);
	pc_check(e == 0, "pthread_join failed (%s)", pc_errname(e));
	pc_pass();
#endif
}
