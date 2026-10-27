/*
 * pthread_attr_init() gives every attribute its default.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_init.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <string.h>

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

static void joinable(pthread_attr_t *a, const char *when)
{
	int r, d = -12345;

	r = pthread_attr_getdetachstate(a, &d);
	pc_check(r == 0, "%s: pthread_attr_getdetachstate() returned %s", when, rn(r));
	pc_check(d == PTHREAD_CREATE_JOINABLE, "%s: detachstate is %d, expected PTHREAD_CREATE_JOINABLE", when, d);
}

int main(void)
{
	static const unsigned char fill[] = { 0x00, 0xff, 0xa5 };
	pthread_attr_t a;
	unsigned i;
	int r;

	pc_begin("XSH.pthread_attr_init.002");
	pc_test();
	for (i = 0; i < sizeof fill; i++) {
		memset(&a, fill[i], sizeof a);
		r = pthread_attr_init(&a);
		pc_check(r == 0, "pthread_attr_init() on storage filled with %#x returned %s", fill[i], rn(r));
		joinable(&a, "after init on filled storage");
		r = pthread_attr_destroy(&a);
		pc_check(r == 0, "pthread_attr_destroy() returned %s", rn(r));
	}
	r = pthread_attr_init(&a);
	pc_check(r == 0, "pthread_attr_init() returned %s", rn(r));
	r = pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);
	pc_check(r == 0, "pthread_attr_setdetachstate(DETACHED) returned %s", rn(r));
	r = pthread_attr_destroy(&a);
	pc_check(r == 0, "pthread_attr_destroy() returned %s", rn(r));
	r = pthread_attr_init(&a);
	pc_check(r == 0, "pthread_attr_init() after the destroy returned %s", rn(r));
	joinable(&a, "after destroy and init of a changed object");
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
