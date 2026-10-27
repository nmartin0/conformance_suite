/*
 * pthread_condattr_init() gives the clock attribute its default, CLOCK_REALTIME.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_condattr_init.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

static void check_default(pthread_condattr_t *a, const char *when)
{
	clockid_t c = (clockid_t)-12345;
	int r;

	r = pthread_condattr_getclock(a, &c);
	pc_check(r == 0, "%s: pthread_condattr_getclock() returned %s", when, rn(r));
	pc_check(c == CLOCK_REALTIME, "%s: the clock attribute is %ld, expected CLOCK_REALTIME (%ld)", when, (long)c,
	    (long)CLOCK_REALTIME);
}

int main(void)
{
	static const unsigned char fill[] = { 0x00, 0xff, 0xa5 };
	pthread_condattr_t a;
	unsigned i;
	int r;

	pc_begin("XSH.pthread_condattr_init.002");
	pc_test();
	for (i = 0; i < sizeof fill; i++) {
		memset(&a, fill[i], sizeof a);
		r = pthread_condattr_init(&a);
		pc_check(r == 0, "pthread_condattr_init() on storage filled with %#x returned %s", fill[i], rn(r));
		check_default(&a, "after init on filled storage");
		(void)pthread_condattr_destroy(&a);
	}
	r = pthread_condattr_init(&a);
	pc_check(r == 0, "pthread_condattr_init() returned %s", rn(r));
	r = pthread_condattr_setclock(&a, CLOCK_MONOTONIC);
	pc_check(r == 0, "pthread_condattr_setclock(CLOCK_MONOTONIC) returned %s", rn(r));
	r = pthread_condattr_destroy(&a);
	pc_check(r == 0, "pthread_condattr_destroy() returned %s", rn(r));
	r = pthread_condattr_init(&a);
	pc_check(r == 0, "pthread_condattr_init() after the destroy returned %s", rn(r));
	check_default(&a, "after destroy and init of a changed object");
	(void)pthread_condattr_destroy(&a);
	pc_pass();
}
