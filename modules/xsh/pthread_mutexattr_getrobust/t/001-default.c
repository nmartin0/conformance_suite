/*
 * The default robust attribute is PTHREAD_MUTEX_STALLED.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_getrobust.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	pthread_mutexattr_t a;
	int v = -12345, r;

	pc_begin("XSH.pthread_mutexattr_getrobust.001");
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	pc_test();
	r = pthread_mutexattr_getrobust(&a, &v);
	pc_check(r == 0, "pthread_mutexattr_getrobust() returned %s", r == 0 ? "0" : pc_errname(r));
	pc_check(v == PTHREAD_MUTEX_STALLED, "a new attributes object has robust %d, expected PTHREAD_MUTEX_STALLED (%d)", v,
	    (int)PTHREAD_MUTEX_STALLED);
	(void)pthread_mutexattr_destroy(&a);
	pc_pass();
}
