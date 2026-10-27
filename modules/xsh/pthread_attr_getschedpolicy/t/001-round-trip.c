/*
 * pthread_attr_getschedpolicy() reports the policy that was set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getschedpolicy.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && _POSIX_THREAD_PRIORITY_SCHEDULING >= 0
static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	static const int pols[] = { SCHED_FIFO, SCHED_RR, SCHED_OTHER, SCHED_RR, SCHED_FIFO };
	static const char *const names[] = { "SCHED_FIFO", "SCHED_RR", "SCHED_OTHER", "SCHED_RR", "SCHED_FIFO" };
	pthread_attr_t a;
	unsigned i;
	int v, r;

	pc_begin("XSH.pthread_attr_getschedpolicy.001");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	pc_test();
	for (i = 0; i < sizeof pols / sizeof pols[0]; i++) {
		r = pthread_attr_setschedpolicy(&a, pols[i]);
		pc_check(r == 0, "pthread_attr_setschedpolicy(%s) returned %s", names[i], rn(r));
		v = -12345;
		r = pthread_attr_getschedpolicy(&a, &v);
		pc_check(r == 0, "pthread_attr_getschedpolicy() returned %s", rn(r));
		pc_check(v == pols[i], "after setting %s (%d), getschedpolicy stored %d", names[i], pols[i], v);
	}
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_attr_getschedpolicy.001");
	pc_unsupported("_POSIX_THREAD_PRIORITY_SCHEDULING is not defined");
}
#endif
