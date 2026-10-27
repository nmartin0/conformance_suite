/*
 * pthread_attr_getdetachstate() reports what pthread_attr_setdetachstate() set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getdetachstate.002
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
	static const int vals[] = { PTHREAD_CREATE_DETACHED, PTHREAD_CREATE_JOINABLE, PTHREAD_CREATE_DETACHED };
	static const char *const names[] = { "DETACHED", "JOINABLE", "DETACHED" };
	pthread_attr_t a;
	unsigned i;
	int d, r;

	pc_begin("XSH.pthread_attr_getdetachstate.002");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	pc_test();
	for (i = 0; i < 3; i++) {
		r = pthread_attr_setdetachstate(&a, vals[i]);
		pc_check(r == 0, "pthread_attr_setdetachstate(PTHREAD_CREATE_%s) returned %s", names[i], rn(r));
		d = -12345;
		r = pthread_attr_getdetachstate(&a, &d);
		pc_check(r == 0, "pthread_attr_getdetachstate() returned %s", rn(r));
		pc_check(d == vals[i], "after setting PTHREAD_CREATE_%s (%d), getdetachstate stored %d", names[i], vals[i], d);
	}
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
