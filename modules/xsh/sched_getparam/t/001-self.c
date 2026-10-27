/*
 * sched_getparam(0) returns a priority inside the range of the policy.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_getparam.001
 */
#include "schedh.h"

int main(void)
{
	struct sched_param sp;
	int pol, lo, hi;

	pc_begin("XSH.sched_getparam.001");
	pol = sched_getscheduler(0);
	if (pol == -1)
		pc_unresolved("sched_getscheduler(0) failed: %s", pc_errname(errno));
	errno = 0;
	lo = sched_get_priority_min(pol);
	if (lo == -1 && errno != 0)
		pc_unresolved("no priority range for policy %d: %s", pol, pc_errname(errno));
	errno = 0;
	hi = sched_get_priority_max(pol);
	if (hi == -1 && errno != 0)
		pc_unresolved("no priority range for policy %d: %s", pol, pc_errname(errno));
	memset(&sp, 0x5a, sizeof sp);
	sp.sched_priority = INT_MIN;
	pc_test();
	pc_expect_ok(sched_getparam(0, &sp), "sched_getparam(0)");
	pc_check(sp.sched_priority >= lo && sp.sched_priority <= hi,
	    "priority %d is outside %d..%d of policy %d", sp.sched_priority, lo, hi, pol);
	pc_pass();
}
