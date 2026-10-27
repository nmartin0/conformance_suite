/*
 * pthread_attr_getstack() reports the stack that pthread_attr_setstack() set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getstack.001
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
static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	long pg = sysconf(_SC_PAGESIZE);
	pthread_attr_t a;
	void *buf = NULL, *addr = NULL;
	size_t size, got = 0;
	int r;

	pc_begin("XSH.pthread_attr_getstack.001");
	NEED(pg > 0, "sysconf(_SC_PAGESIZE) failed");
	size = ((size_t)PTHREAD_STACK_MIN + 65536 + (size_t)pg - 1) / (size_t)pg * (size_t)pg;
	NEED(posix_memalign(&buf, (size_t)pg, size) == 0, "posix_memalign failed");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	pc_test();
	r = pthread_attr_setstack(&a, buf, size);
	if (r == EINVAL || r == EACCES)
		pc_unresolved("pthread_attr_setstack() refused the region: %s", pc_errname(r));
	pc_check(r == 0, "pthread_attr_setstack() returned %s", rn(r));
	r = pthread_attr_getstack(&a, &addr, &got);
	pc_check(r == 0, "pthread_attr_getstack() returned %s", rn(r));
	pc_check(addr == buf, "getstack stored address %p, expected %p", addr, buf);
	pc_check(got == size, "getstack stored size %zu, expected %zu", got, size);
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_attr_getstack.001");
	pc_unsupported("the thread stack address and size attributes are not defined");
}
#endif
