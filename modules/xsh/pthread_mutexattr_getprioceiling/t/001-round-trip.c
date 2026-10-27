/*
 * pthread_mutexattr_getprioceiling() reports the ceiling that was set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_getprioceiling.001
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
	int want[2], i, v, r;

	pc_begin("XSH.pthread_mutexattr_getprioceiling.001");
	want[0] = sched_get_priority_min(SCHED_FIFO);
	want[1] = sched_get_priority_max(SCHED_FIFO);
	NEED(want[0] != -1 && want[1] != -1, "sched_get_priority_min/max(SCHED_FIFO) failed");
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	pc_test();
	for (i = 0; i < 2; i++) {
		r = pthread_mutexattr_setprioceiling(&a, want[i]);
		if (r == EINVAL || r == EPERM)
			pc_unresolved("pthread_mutexattr_setprioceiling(%d) was refused: %s", want[i], pc_errname(r));
		pc_check(r == 0, "pthread_mutexattr_setprioceiling(%d) returned %s", want[i], rn(r));
		v = -12345;
		r = pthread_mutexattr_getprioceiling(&a, &v);
		pc_check(r == 0, "pthread_mutexattr_getprioceiling() returned %s", rn(r));
		pc_check(v == want[i], "after setting the ceiling %d, getprioceiling stored %d", want[i], v);
	}
	(void)pthread_mutexattr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_mutexattr_getprioceiling.001");
	pc_unsupported("no priority protection option is defined");
}
#endif
