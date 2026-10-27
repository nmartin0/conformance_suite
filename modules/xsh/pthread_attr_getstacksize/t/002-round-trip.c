/*
 * pthread_attr_getstacksize() reports the size that was set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getstacksize.002
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if defined(_POSIX_THREAD_ATTR_STACKSIZE) && _POSIX_THREAD_ATTR_STACKSIZE >= 0 && defined(PTHREAD_STACK_MIN)
static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	pthread_attr_t a;
	size_t want[3], got;
	int i, r;

	pc_begin("XSH.pthread_attr_getstacksize.002");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	pc_test();
	want[0] = (size_t)PTHREAD_STACK_MIN;
	r = pthread_attr_setstacksize(&a, want[0]);
	pc_check(r == 0, "pthread_attr_setstacksize({PTHREAD_STACK_MIN} = %zu) returned %s", want[0], rn(r));
	got = 0;
	r = pthread_attr_getstacksize(&a, &got);
	pc_check(r == 0, "pthread_attr_getstacksize() returned %s", rn(r));
	pc_check(got == want[0], "after setting %zu, getstacksize stored %zu", want[0], got);

	NEED(pthread_attr_destroy(&a) == 0 && pthread_attr_init(&a) == 0, "re-initializing the attributes object failed");
	r = pthread_attr_getstacksize(&a, &want[1]);
	pc_check(r == 0, "pthread_attr_getstacksize() on a new object returned %s", rn(r));
	want[2] = (size_t)PTHREAD_STACK_MIN + 1048576;
	for (i = 1; i < 3; i++) {
		r = pthread_attr_setstacksize(&a, want[i]);
		if (r == EINVAL)
			continue;	/* above a system-imposed limit: not checked */
		pc_check(r == 0, "pthread_attr_setstacksize(%zu) returned %s", want[i], rn(r));
		got = 0;
		r = pthread_attr_getstacksize(&a, &got);
		pc_check(r == 0, "pthread_attr_getstacksize() returned %s", rn(r));
		pc_check(got == want[i], "after setting %zu, getstacksize stored %zu", want[i], got);
	}
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_attr_getstacksize.002");
	pc_unsupported("the thread stack size attribute or {PTHREAD_STACK_MIN} is not defined");
}
#endif
