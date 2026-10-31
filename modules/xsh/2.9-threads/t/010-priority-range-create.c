/*
 * An attributes object whose priority is outside the range of its policy
 * does not give a thread with a priority outside that range.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.010
 *
 * The attributes ask for explicit scheduling with a priority-based policy
 * of the option (the creator's own when it is SCHED_FIFO or SCHED_RR,
 * SCHED_FIFO otherwise: the range rule covers only those policies) and one
 * more than that policy's highest priority. If the attributes
 * object already refuses the policy or the priority the test is not in use
 * (the value never reaches thread creation). pthread_create() may fail (the
 * text names no code for this) or may create a thread whose priority has
 * been brought into range; it may not create one outside the range.
 */
#include "pcth.h"

static void *body(void *a)
{
	struct sched_param sp;
	int policy;

	(void)a;
	if (pthread_getschedparam(pthread_self(), &policy, &sp) == 0) {
		flag_set(1, policy);
		flag_set(2, sp.sched_priority);
		flag_set(0, 1);
	} else {
		flag_set(0, 2);
	}
	return NULL;
}

int main(void)
{
	pthread_attr_t a;
	pthread_t t;
	struct sched_param sp;
	int policy, r, max;

	pc_begin("XSH.2.9-threads.010");
	guard(60);
	NEED(pthread_getschedparam(pthread_self(), &policy, &sp) == 0, "pthread_getschedparam failed");
	if (policy != SCHED_FIFO && policy != SCHED_RR)
		policy = SCHED_FIFO;
	max = sched_get_priority_max(policy);
	NEED(max != -1, "sched_get_priority_max failed");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	pc_test();
	NEED(pthread_attr_setinheritsched(&a, PTHREAD_EXPLICIT_SCHED) == 0, "pthread_attr_setinheritsched failed");
	r = pthread_attr_setschedpolicy(&a, policy);
	if (r != 0)
		pc_notinuse("pthread_attr_setschedpolicy() refuses policy %d (%s)", policy, pc_errname(r));
	sp.sched_priority = max + 1;
	r = pthread_attr_setschedparam(&a, &sp);
	if (r != 0)
		pc_notinuse("pthread_attr_setschedparam() refuses a priority outside the range (%s)", pc_errname(r));
	r = pthread_create(&t, &a, body, NULL);
	if (r == 0) {
		int p, min, mx;

		NEED(join_within(t, 10000, NULL), "the new thread did not end");
		NEED(flag_get(0) == 1, "pthread_getschedparam failed in the new thread");
		p = flag_get(2);
		min = sched_get_priority_min(flag_get(1));
		mx = sched_get_priority_max(flag_get(1));
		NEED(min != -1 && mx != -1, "sched_get_priority_min or _max failed");
		pc_check(p >= min && p <= mx, "pthread_create() made a thread with priority %d, outside the range %d to %d of its policy", p, min, mx);
	}
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
