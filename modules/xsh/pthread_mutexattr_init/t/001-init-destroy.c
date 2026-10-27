/*
 * pthread_mutexattr_init() and pthread_mutexattr_destroy() succeed, also in sequence.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_init.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	pthread_mutexattr_t a;
	int r, i;

	pc_begin("XSH.pthread_mutexattr_init.001");
	pc_test();
	for (i = 0; i < 2; i++) {
		r = pthread_mutexattr_init(&a);
		pc_check(r == 0, "pthread_mutexattr_init() (round %d) returned %s", i + 1, rn(r));
		r = pthread_mutexattr_destroy(&a);
		pc_check(r == 0, "pthread_mutexattr_destroy() (round %d) returned %s", i + 1, rn(r));
	}
	pc_pass();
}
