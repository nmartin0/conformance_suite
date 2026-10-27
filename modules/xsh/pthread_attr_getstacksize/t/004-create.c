/*
 * A thread can be created with the smallest stack size.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getstacksize.004
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if defined(_POSIX_THREAD_ATTR_STACKSIZE) && _POSIX_THREAD_ATTR_STACKSIZE >= 0 && defined(PTHREAD_STACK_MIN)
static int token;

static void *routine(void *arg)
{
	return arg;
}

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	pthread_attr_t a;
	pthread_t t;
	void *out = NULL;
	int r;

	pc_begin("XSH.pthread_attr_getstacksize.004");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	NEED(pthread_attr_setstacksize(&a, (size_t)PTHREAD_STACK_MIN) == 0, "pthread_attr_setstacksize({PTHREAD_STACK_MIN}) failed");
	pc_test();
	r = pthread_create(&t, &a, routine, &token);
	if (r == EAGAIN)
		pc_unresolved("pthread_create() failed with EAGAIN");
	pc_check(r == 0, "pthread_create() with stack size {PTHREAD_STACK_MIN} returned %s", rn(r));
	r = pthread_join(t, &out);
	pc_check(r == 0, "pthread_join() returned %s", rn(r));
	pc_check(out == &token, "the thread returned %p, expected %p", out, (void *)&token);
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_attr_getstacksize.004");
	pc_unsupported("the thread stack size attribute or {PTHREAD_STACK_MIN} is not defined");
}
#endif
