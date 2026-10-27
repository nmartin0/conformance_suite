/*
 * A thread created with an application-managed stack runs on it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getstack.002
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if defined(_POSIX_THREAD_ATTR_STACKADDR) && _POSIX_THREAD_ATTR_STACKADDR >= 0 && \
    defined(_POSIX_THREAD_ATTR_STACKSIZE) && _POSIX_THREAD_ATTR_STACKSIZE >= 0 && defined(PTHREAD_STACK_MIN)
static void *routine(void *arg)
{
	volatile char marker = 0;

	*(uintptr_t *)arg = (uintptr_t)&marker;
	return arg;
}

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	long pg = sysconf(_SC_PAGESIZE);
	pthread_attr_t a;
	pthread_t t;
	void *buf = NULL, *out = NULL;
	uintptr_t where = 0, lo;
	size_t size;
	int r;

	pc_begin("XSH.pthread_attr_getstack.002");
	NEED(pg > 0, "sysconf(_SC_PAGESIZE) failed");
	size = ((size_t)PTHREAD_STACK_MIN + 65536 + (size_t)pg - 1) / (size_t)pg * (size_t)pg;
	NEED(posix_memalign(&buf, (size_t)pg, size) == 0, "posix_memalign failed");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	r = pthread_attr_setstack(&a, buf, size);
	NEED(r == 0, "pthread_attr_setstack() returned %s", rn(r));
	pc_test();
	r = pthread_create(&t, &a, routine, &where);
	if (r == EAGAIN)
		pc_unresolved("pthread_create() failed with EAGAIN");
	pc_check(r == 0, "pthread_create() with an application-managed stack returned %s", rn(r));
	r = pthread_join(t, &out);
	pc_check(r == 0, "pthread_join() returned %s", rn(r));
	pc_check(out == &where, "the start routine's return value was not handed back");
	lo = (uintptr_t)buf;
	pc_check(where >= lo && where < lo + size, "an automatic variable of the thread is at %#lx, outside the stack [%#lx, %#lx)",
	    (unsigned long)where, (unsigned long)lo, (unsigned long)(lo + size));
	/* The memory now belongs to the implementation (XSH 2.9.8); it is not freed. */
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_attr_getstack.002");
	pc_unsupported("the thread stack address and size attributes are not defined");
}
#endif
