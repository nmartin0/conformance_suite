/*
 * The default contention scope is one of the two defined values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getscope.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && _POSIX_THREAD_PRIORITY_SCHEDULING >= 0
int main(void)
{
	pthread_attr_t a;
	int v = -12345, r;

	pc_begin("XSH.pthread_attr_getscope.001");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	pc_test();
	r = pthread_attr_getscope(&a, &v);
	pc_check(r == 0, "pthread_attr_getscope() on a new object returned %s", r == 0 ? "0" : pc_errname(r));
	pc_check(v == PTHREAD_SCOPE_SYSTEM || v == PTHREAD_SCOPE_PROCESS,
	    "the default contention scope is %d, neither PTHREAD_SCOPE_SYSTEM nor PTHREAD_SCOPE_PROCESS", v);
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_attr_getscope.001");
	pc_unsupported("_POSIX_THREAD_PRIORITY_SCHEDULING is not defined");
}
#endif
