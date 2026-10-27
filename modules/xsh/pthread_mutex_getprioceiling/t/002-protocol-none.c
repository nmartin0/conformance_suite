/*
 * The priority ceiling of a mutex without the protect protocol is refused.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutex_getprioceiling.002
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
	pthread_mutex_t m;
	int v = -12345, old = -12345, hi, r;

	pc_begin("XSH.pthread_mutex_getprioceiling.002");
	hi = sched_get_priority_max(SCHED_FIFO);
	NEED(hi != -1, "sched_get_priority_max(SCHED_FIFO) failed");
	NEED(pthread_mutex_init(&m, NULL) == 0, "pthread_mutex_init failed");
	pc_test();
	r = pthread_mutex_getprioceiling(&m, &v);
	pc_check(r == EINVAL, "pthread_mutex_getprioceiling() of a PTHREAD_PRIO_NONE mutex returned %s, expected EINVAL", rn(r));
	r = pthread_mutex_setprioceiling(&m, hi, &old);
	pc_check(r == EINVAL, "pthread_mutex_setprioceiling() of a PTHREAD_PRIO_NONE mutex returned %s, expected EINVAL", rn(r));
	(void)pthread_mutex_destroy(&m);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_mutex_getprioceiling.002");
	pc_unsupported("no priority protection option is defined");
}
#endif
