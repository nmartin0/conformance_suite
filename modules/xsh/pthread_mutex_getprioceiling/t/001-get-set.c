/*
 * A priority-protect mutex reports and changes its priority ceiling.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutex_getprioceiling.001
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

static void *trylock_main(void *p)
{
	int r = pthread_mutex_trylock(p);

	if (r == 0)
		(void)pthread_mutex_unlock(p);
	return (void *)(long)r;
}

int main(void)
{
	pthread_mutexattr_t a;
	pthread_mutex_t m;
	pthread_t t;
	void *out;
	int lo, hi, v, old, r;

	pc_begin("XSH.pthread_mutex_getprioceiling.001");
	lo = sched_get_priority_min(SCHED_FIFO);
	hi = sched_get_priority_max(SCHED_FIFO);
	NEED(lo != -1 && hi != -1 && lo != hi, "sched_get_priority_min/max(SCHED_FIFO) unusable (%d, %d)", lo, hi);
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	r = pthread_mutexattr_setprotocol(&a, PTHREAD_PRIO_PROTECT);
	NEED(r == 0, "pthread_mutexattr_setprotocol(PTHREAD_PRIO_PROTECT) returned %s", rn(r));
	r = pthread_mutexattr_setprioceiling(&a, hi);
	NEED(r == 0, "pthread_mutexattr_setprioceiling(%d) returned %s", hi, rn(r));
	r = pthread_mutex_init(&m, &a);
	NEED(r == 0, "pthread_mutex_init() returned %s", rn(r));
	pc_test();
	v = -12345;
	r = pthread_mutex_getprioceiling(&m, &v);
	pc_check(r == 0, "pthread_mutex_getprioceiling() returned %s", rn(r));
	pc_check(v == hi, "the ceiling is %d, expected %d", v, hi);
	old = -12345;
	r = pthread_mutex_setprioceiling(&m, lo, &old);
	if (r == EPERM || r == EINVAL || r == ENOTSUP)
		pc_unresolved("pthread_mutex_setprioceiling(%d) returned %s", lo, pc_errname(r));
	pc_check(r == 0, "pthread_mutex_setprioceiling() returned %s", rn(r));
	pc_check(old == hi, "the previous ceiling stored is %d, expected %d", old, hi);
	NEED(pthread_create(&t, NULL, trylock_main, &m) == 0, "pthread_create failed");
	NEED(pthread_join(t, &out) == 0, "pthread_join failed");
	if ((long)out == EINVAL)
		pc_unresolved("the other thread's trylock returned EINVAL (its priority exceeds the lowered ceiling)");
	pc_check((long)out == 0, "after pthread_mutex_setprioceiling() another thread's trylock returned %s, expected 0 (unlocked)",
	    rn((int)(long)out));
	v = -12345;
	r = pthread_mutex_getprioceiling(&m, &v);
	pc_check(r == 0, "pthread_mutex_getprioceiling() returned %s", rn(r));
	pc_check(v == lo, "the ceiling is %d after setting %d", v, lo);
	(void)pthread_mutex_destroy(&m);
	(void)pthread_mutexattr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_mutex_getprioceiling.001");
	pc_unsupported("no priority protection option is defined");
}
#endif
