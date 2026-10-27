/*
 * pthread_attr_setstack() rejects a stack smaller than {PTHREAD_STACK_MIN}.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getstack.003
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if defined(_POSIX_THREAD_ATTR_STACKADDR) && _POSIX_THREAD_ATTR_STACKADDR >= 0 && \
    defined(_POSIX_THREAD_ATTR_STACKSIZE) && _POSIX_THREAD_ATTR_STACKSIZE >= 0 && defined(PTHREAD_STACK_MIN)
int main(void)
{
	long pg = sysconf(_SC_PAGESIZE);
	pthread_attr_t a;
	void *buf = NULL;
	size_t min = (size_t)PTHREAD_STACK_MIN, alloc;
	int r;

	pc_begin("XSH.pthread_attr_getstack.003");
	NEED(pg > 0, "sysconf(_SC_PAGESIZE) failed");
	NEED(min > 1, "{PTHREAD_STACK_MIN} is %zu", min);
	alloc = (min + (size_t)pg - 1) / (size_t)pg * (size_t)pg;
	NEED(posix_memalign(&buf, (size_t)pg, alloc) == 0, "posix_memalign failed");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	pc_test();
	r = pthread_attr_setstack(&a, buf, min - 1);
	pc_check(r == EINVAL, "pthread_attr_setstack() with size {PTHREAD_STACK_MIN} - 1 returned %s, expected EINVAL",
	    r == 0 ? "0" : pc_errname(r));
	(void)pthread_attr_destroy(&a);
	free(buf);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_attr_getstack.003");
	pc_unsupported("the thread stack address and size attributes are not defined");
}
#endif
