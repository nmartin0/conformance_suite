/*
 * pthread_mutexattr_getrobust() reports what pthread_mutexattr_setrobust() set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_getrobust.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	static const int vals[] = { PTHREAD_MUTEX_ROBUST, PTHREAD_MUTEX_STALLED, PTHREAD_MUTEX_ROBUST };
	static const char *const names[] = { "ROBUST", "STALLED", "ROBUST" };
	pthread_mutexattr_t a;
	unsigned i;
	int v, r;

	pc_begin("XSH.pthread_mutexattr_getrobust.002");
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	pc_test();
	for (i = 0; i < 3; i++) {
		r = pthread_mutexattr_setrobust(&a, vals[i]);
		pc_check(r == 0, "pthread_mutexattr_setrobust(PTHREAD_MUTEX_%s) returned %s", names[i], rn(r));
		v = -12345;
		r = pthread_mutexattr_getrobust(&a, &v);
		pc_check(r == 0, "pthread_mutexattr_getrobust() returned %s", rn(r));
		pc_check(v == vals[i], "after setting PTHREAD_MUTEX_%s (%d), getrobust stored %d", names[i], vals[i], v);
	}
	(void)pthread_mutexattr_destroy(&a);
	pc_pass();
}
