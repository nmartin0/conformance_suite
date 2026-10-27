/*
 * pthread_condattr_getclock() reports what pthread_condattr_setclock() set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_condattr_getclock.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <time.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	static const clockid_t vals[] = { CLOCK_MONOTONIC, CLOCK_REALTIME, CLOCK_MONOTONIC };
	static const char *const names[] = { "CLOCK_MONOTONIC", "CLOCK_REALTIME", "CLOCK_MONOTONIC" };
	pthread_condattr_t a;
	clockid_t c;
	unsigned i;
	int r;

	pc_begin("XSH.pthread_condattr_getclock.002");
	NEED(pthread_condattr_init(&a) == 0, "pthread_condattr_init failed");
	pc_test();
	for (i = 0; i < 3; i++) {
		r = pthread_condattr_setclock(&a, vals[i]);
		pc_check(r == 0, "pthread_condattr_setclock(%s) returned %s", names[i], rn(r));
		c = (clockid_t)-12345;
		r = pthread_condattr_getclock(&a, &c);
		pc_check(r == 0, "pthread_condattr_getclock() returned %s", rn(r));
		pc_check(c == vals[i], "after setting %s (%ld), getclock stored %ld", names[i], (long)vals[i], (long)c);
	}
	(void)pthread_condattr_destroy(&a);
	pc_pass();
}
