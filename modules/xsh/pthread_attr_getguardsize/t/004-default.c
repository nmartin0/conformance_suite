/*
 * pthread_attr_getguardsize() works on a new attributes object.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getguardsize.004
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	pthread_attr_t a;
	size_t g = 0;
	int r;

	pc_begin("XSH.pthread_attr_getguardsize.004");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	pc_test();
	r = pthread_attr_getguardsize(&a, &g);
	pc_check(r == 0, "pthread_attr_getguardsize() on a new object returned %s", r == 0 ? "0" : pc_errname(r));
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
