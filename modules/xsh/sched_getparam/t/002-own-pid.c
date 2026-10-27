/*
 * sched_getparam() of the caller's pid gives the same as pid 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_getparam.002
 */
#include "schedh.h"

int main(void)
{
	struct sched_param a, b;

	pc_begin("XSH.sched_getparam.002");
	memset(&a, 0, sizeof a);
	memset(&b, 0, sizeof b);
	a.sched_priority = INT_MIN;
	b.sched_priority = INT_MIN + 1;
	pc_test();
	pc_expect_ok(sched_getparam(0, &a), "sched_getparam(0)");
	pc_expect_ok(sched_getparam(getpid(), &b), "sched_getparam(getpid())");
	pc_check(a.sched_priority == b.sched_priority, "pid 0 gives %d, getpid() gives %d",
	    a.sched_priority, b.sched_priority);
	pc_pass();
}
