/*
 * The default detachstate is PTHREAD_CREATE_JOINABLE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getdetachstate.001
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
	pthread_attr_t a;
	int d = -12345, r;

	pc_begin("XSH.pthread_attr_getdetachstate.001");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	pc_test();
	r = pthread_attr_getdetachstate(&a, &d);
	pc_check(r == 0, "pthread_attr_getdetachstate() returned %s", rn(r));
	pc_check(d == PTHREAD_CREATE_JOINABLE, "a new attributes object has detachstate %d, expected PTHREAD_CREATE_JOINABLE (%d)",
	    d, (int)PTHREAD_CREATE_JOINABLE);
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
