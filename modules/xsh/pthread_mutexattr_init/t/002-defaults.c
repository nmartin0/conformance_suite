/*
 * pthread_mutexattr_init() gives the type and robust attributes their defaults.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_init.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <string.h>

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

static void check_defaults(pthread_mutexattr_t *a, const char *when)
{
	int t = -12345, rb = -12345, r;

	r = pthread_mutexattr_gettype(a, &t);
	pc_check(r == 0, "%s: pthread_mutexattr_gettype() returned %s", when, rn(r));
	pc_check(t == PTHREAD_MUTEX_DEFAULT, "%s: type is %d, expected PTHREAD_MUTEX_DEFAULT (%d)", when, t,
	    (int)PTHREAD_MUTEX_DEFAULT);
	r = pthread_mutexattr_getrobust(a, &rb);
	pc_check(r == 0, "%s: pthread_mutexattr_getrobust() returned %s", when, rn(r));
	pc_check(rb == PTHREAD_MUTEX_STALLED, "%s: robust is %d, expected PTHREAD_MUTEX_STALLED (%d)", when, rb,
	    (int)PTHREAD_MUTEX_STALLED);
}

int main(void)
{
	static const unsigned char fill[] = { 0x00, 0xff, 0xa5 };
	pthread_mutexattr_t a;
	unsigned i;
	int r;

	pc_begin("XSH.pthread_mutexattr_init.002");
	pc_test();
	for (i = 0; i < sizeof fill; i++) {
		memset(&a, fill[i], sizeof a);
		r = pthread_mutexattr_init(&a);
		pc_check(r == 0, "pthread_mutexattr_init() on storage filled with %#x returned %s", fill[i], rn(r));
		check_defaults(&a, "after init on filled storage");
		(void)pthread_mutexattr_destroy(&a);
	}
	r = pthread_mutexattr_init(&a);
	pc_check(r == 0, "pthread_mutexattr_init() returned %s", rn(r));
	r = pthread_mutexattr_settype(&a, PTHREAD_MUTEX_ERRORCHECK);
	pc_check(r == 0, "pthread_mutexattr_settype(ERRORCHECK) returned %s", rn(r));
	r = pthread_mutexattr_setrobust(&a, PTHREAD_MUTEX_ROBUST);
	pc_check(r == 0, "pthread_mutexattr_setrobust(ROBUST) returned %s", rn(r));
	r = pthread_mutexattr_destroy(&a);
	pc_check(r == 0, "pthread_mutexattr_destroy() returned %s", rn(r));
	r = pthread_mutexattr_init(&a);
	pc_check(r == 0, "pthread_mutexattr_init() after the destroy returned %s", rn(r));
	check_defaults(&a, "after destroy and init of a changed object");
	(void)pthread_mutexattr_destroy(&a);
	pc_pass();
}
