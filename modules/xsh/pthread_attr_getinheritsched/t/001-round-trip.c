/*
 * pthread_attr_getinheritsched() reports what pthread_attr_setinheritsched() set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getinheritsched.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && _POSIX_THREAD_PRIORITY_SCHEDULING >= 0
static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	static const int vals[] = { PTHREAD_INHERIT_SCHED, PTHREAD_EXPLICIT_SCHED, PTHREAD_INHERIT_SCHED };
	static const char *const names[] = { "INHERIT_SCHED", "EXPLICIT_SCHED", "INHERIT_SCHED" };
	pthread_attr_t a;
	unsigned i;
	int v, r;

	pc_begin("XSH.pthread_attr_getinheritsched.001");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	pc_test();
	for (i = 0; i < 3; i++) {
		r = pthread_attr_setinheritsched(&a, vals[i]);
		pc_check(r == 0, "pthread_attr_setinheritsched(PTHREAD_%s) returned %s", names[i], rn(r));
		v = -12345;
		r = pthread_attr_getinheritsched(&a, &v);
		pc_check(r == 0, "pthread_attr_getinheritsched() returned %s", rn(r));
		pc_check(v == vals[i], "after setting PTHREAD_%s (%d), getinheritsched stored %d", names[i], vals[i], v);
	}
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_attr_getinheritsched.001");
	pc_unsupported("_POSIX_THREAD_PRIORITY_SCHEDULING is not defined");
}
#endif
