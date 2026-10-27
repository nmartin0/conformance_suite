/*
 * The default clock attribute of a condition variable is CLOCK_REALTIME.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_condattr_getclock.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <time.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	pthread_condattr_t a;
	clockid_t c = (clockid_t)-12345;
	int r;

	pc_begin("XSH.pthread_condattr_getclock.001");
	NEED(pthread_condattr_init(&a) == 0, "pthread_condattr_init failed");
	pc_test();
	r = pthread_condattr_getclock(&a, &c);
	pc_check(r == 0, "pthread_condattr_getclock() returned %s", r == 0 ? "0" : pc_errname(r));
	pc_check(c == CLOCK_REALTIME, "a new attributes object has clock %ld, expected CLOCK_REALTIME (%ld)", (long)c,
	    (long)CLOCK_REALTIME);
	(void)pthread_condattr_destroy(&a);
	pc_pass();
}
