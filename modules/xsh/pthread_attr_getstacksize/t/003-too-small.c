/*
 * pthread_attr_setstacksize() rejects a size below {PTHREAD_STACK_MIN}.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getstacksize.003
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if defined(_POSIX_THREAD_ATTR_STACKSIZE) && _POSIX_THREAD_ATTR_STACKSIZE >= 0 && defined(PTHREAD_STACK_MIN)
int main(void)
{
	pthread_attr_t a;
	size_t min = (size_t)PTHREAD_STACK_MIN;
	int r;

	pc_begin("XSH.pthread_attr_getstacksize.003");
	NEED(min > 1, "{PTHREAD_STACK_MIN} is %zu", min);
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	pc_test();
	r = pthread_attr_setstacksize(&a, min - 1);
	pc_check(r == EINVAL, "pthread_attr_setstacksize({PTHREAD_STACK_MIN} - 1) returned %s, expected EINVAL",
	    r == 0 ? "0" : pc_errname(r));
	r = pthread_attr_setstacksize(&a, 0);
	pc_check(r == EINVAL, "pthread_attr_setstacksize(0) returned %s, expected EINVAL", r == 0 ? "0" : pc_errname(r));
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_attr_getstacksize.003");
	pc_unsupported("the thread stack size attribute or {PTHREAD_STACK_MIN} is not defined");
}
#endif
