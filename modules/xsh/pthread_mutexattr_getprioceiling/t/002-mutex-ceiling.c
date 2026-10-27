/*
 * A mutex made from an attributes object has the object's priority ceiling.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_getprioceiling.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if (defined(_POSIX_THREAD_PRIO_PROTECT) && _POSIX_THREAD_PRIO_PROTECT >= 0) || \
    (defined(_POSIX_THREAD_ROBUST_PRIO_PROTECT) && _POSIX_THREAD_ROBUST_PRIO_PROTECT >= 0)
static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	pthread_mutexattr_t a;
	pthread_mutex_t m;
	int max, v = -12345, r;

	pc_begin("XSH.pthread_mutexattr_getprioceiling.002");
	max = sched_get_priority_max(SCHED_FIFO);
	NEED(max != -1, "sched_get_priority_max(SCHED_FIFO) failed");
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	r = pthread_mutexattr_setprotocol(&a, PTHREAD_PRIO_PROTECT);
	NEED(r == 0, "pthread_mutexattr_setprotocol(PTHREAD_PRIO_PROTECT) returned %s", rn(r));
	r = pthread_mutexattr_setprioceiling(&a, max);
	NEED(r == 0, "pthread_mutexattr_setprioceiling(%d) returned %s", max, rn(r));
	r = pthread_mutex_init(&m, &a);
	NEED(r == 0, "pthread_mutex_init() returned %s", rn(r));
	pc_test();
	r = pthread_mutex_getprioceiling(&m, &v);
	pc_check(r == 0, "pthread_mutex_getprioceiling() returned %s", rn(r));
	pc_check(v == max, "the mutex has priority ceiling %d, expected %d", v, max);
	(void)pthread_mutex_destroy(&m);
	(void)pthread_mutexattr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_mutexattr_getprioceiling.002");
	pc_unsupported("no priority protection option is defined");
}
#endif
