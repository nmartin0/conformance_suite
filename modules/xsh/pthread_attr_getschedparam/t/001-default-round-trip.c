/*
 * The scheduling parameters of an attributes object read back as set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getschedparam.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <sched.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	pthread_attr_t a;
	struct sched_param p, q;
	int r;

	pc_begin("XSH.pthread_attr_getschedparam.001");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	p.sched_priority = -12345;
	pc_test();
	r = pthread_attr_getschedparam(&a, &p);
	pc_check(r == 0, "pthread_attr_getschedparam() on a new object returned %s", rn(r));
	r = pthread_attr_setschedparam(&a, &p);
	if (r == EINVAL || r == ENOTSUP)
		pc_unresolved("pthread_attr_setschedparam() refused the default parameters: %s", pc_errname(r));
	pc_check(r == 0, "pthread_attr_setschedparam() with the default parameters returned %s", rn(r));
	q.sched_priority = -54321;
	r = pthread_attr_getschedparam(&a, &q);
	pc_check(r == 0, "pthread_attr_getschedparam() returned %s", rn(r));
	pc_check(q.sched_priority == p.sched_priority, "sched_priority is %d after storing %d", q.sched_priority,
	    p.sched_priority);
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
