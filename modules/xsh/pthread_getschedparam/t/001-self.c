/*
 * pthread_getschedparam() of the calling thread gives a policy and a
 * priority in its range.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_getschedparam.001
 */
#include "schedh.h"

int main(void)
{
	int pol, prio, lo, hi, e;

	pc_begin("XSH.pthread_getschedparam.001");
	pc_test();
	e = sh_tget(pthread_self(), &pol, &prio);
	pc_check(e == 0, "pthread_getschedparam failed: %s", pc_errname(e));
	errno = 0;
	lo = sched_get_priority_min(pol);
	if (lo == -1 && errno != 0)
		pc_unresolved("no priority range for policy %d: %s", pol, pc_errname(errno));
	errno = 0;
	hi = sched_get_priority_max(pol);
	if (hi == -1 && errno != 0)
		pc_unresolved("no priority range for policy %d: %s", pol, pc_errname(errno));
	pc_check(prio >= lo && prio <= hi, "priority %d is outside %d..%d of policy %d", prio, lo, hi, pol);
	pc_pass();
}
