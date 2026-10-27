/*
 * pthread_attr_setdetachstate() rejects a value that is neither constant.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getdetachstate.003
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
	static const int cand[] = { 0x7ffffff0, -0x7ffffff0, 0x1234567 };
	pthread_attr_t a;
	unsigned i;
	int r;

	pc_begin("XSH.pthread_attr_getdetachstate.003");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	for (i = 0; i < 3; i++)
		NEED(cand[i] != PTHREAD_CREATE_DETACHED && cand[i] != PTHREAD_CREATE_JOINABLE,
		    "%#x is the value of a standard detachstate here", (unsigned)cand[i]);
	pc_test();
	for (i = 0; i < 3; i++) {
		r = pthread_attr_setdetachstate(&a, cand[i]);
		pc_check(r == EINVAL, "pthread_attr_setdetachstate(%d) returned %s, expected EINVAL", cand[i], rn(r));
	}
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
