/*
 * pthread_attr_getstacksize() works on a new attributes object.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getstacksize.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if defined(_POSIX_THREAD_ATTR_STACKSIZE) && _POSIX_THREAD_ATTR_STACKSIZE >= 0
int main(void)
{
	pthread_attr_t a;
	size_t s = 0;
	int r;

	pc_begin("XSH.pthread_attr_getstacksize.001");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	pc_test();
	r = pthread_attr_getstacksize(&a, &s);
	pc_check(r == 0, "pthread_attr_getstacksize() on a new object returned %s", r == 0 ? "0" : pc_errname(r));
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_attr_getstacksize.001");
	pc_unsupported("_POSIX_THREAD_ATTR_STACKSIZE is not defined");
}
#endif
