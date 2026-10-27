/*
 * SCHED_FIFO priorities at both ends of the range read back as set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getschedparam.002
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
	pthread_attr_t a;
	struct sched_param p;
	int want[2], i, r;

	pc_begin("XSH.pthread_attr_getschedparam.002");
	want[0] = sched_get_priority_min(SCHED_FIFO);
	want[1] = sched_get_priority_max(SCHED_FIFO);
	NEED(want[0] != -1 && want[1] != -1, "sched_get_priority_min/max(SCHED_FIFO) failed");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	NEED(pthread_attr_setschedpolicy(&a, SCHED_FIFO) == 0, "pthread_attr_setschedpolicy(SCHED_FIFO) failed");
	pc_test();
	for (i = 0; i < 2; i++) {
		p.sched_priority = want[i];
		r = pthread_attr_setschedparam(&a, &p);
		if (r == EINVAL || r == ENOTSUP || r == EPERM)
			pc_unresolved("pthread_attr_setschedparam(%d) was refused: %s", want[i], pc_errname(r));
		pc_check(r == 0, "pthread_attr_setschedparam(%d) returned %s", want[i], rn(r));
		p.sched_priority = -12345;
		r = pthread_attr_getschedparam(&a, &p);
		pc_check(r == 0, "pthread_attr_getschedparam() returned %s", rn(r));
		pc_check(p.sched_priority == want[i], "sched_priority is %d after setting %d", p.sched_priority, want[i]);
	}
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_attr_getschedparam.002");
	pc_unsupported("_POSIX_THREAD_PRIORITY_SCHEDULING is not defined");
}
#endif
