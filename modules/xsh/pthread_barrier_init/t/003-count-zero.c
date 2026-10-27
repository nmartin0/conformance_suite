/*
 * pthread_barrier_init() rejects a count of 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_barrier_init.003
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

int main(void)
{
	pthread_barrier_t b;
	int r;

	pc_begin("XSH.pthread_barrier_init.003");
	pc_test();
	r = pthread_barrier_init(&b, NULL, 0);
	pc_check(r == EINVAL, "pthread_barrier_init() with count 0 returned %s, expected EINVAL", r == 0 ? "0" : pc_errname(r));
	pc_pass();
}
