/*
 * A guard size of 0 is accepted and reported.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getguardsize.001
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
	size_t g = 12345;
	int r;

	pc_begin("XSH.pthread_attr_getguardsize.001");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	pc_test();
	r = pthread_attr_setguardsize(&a, 0);
	pc_check(r == 0, "pthread_attr_setguardsize(0) returned %s", rn(r));
	r = pthread_attr_getguardsize(&a, &g);
	pc_check(r == 0, "pthread_attr_getguardsize() returned %s", rn(r));
	pc_check(g == 0, "after setguardsize(0), getguardsize stored %zu", g);
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
