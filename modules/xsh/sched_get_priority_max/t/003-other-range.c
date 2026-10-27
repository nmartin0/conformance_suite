/*
 * The SCHED_OTHER priority range exists and is not inverted.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_get_priority_max.003
 */
#include "pc.h"
#include <errno.h>
#include <sched.h>

int main(void)
{
	int lo, hi;

	pc_begin("XSH.sched_get_priority_max.003");
	pc_test();
	errno = 0;
	lo = sched_get_priority_min(SCHED_OTHER);
	pc_check(!(lo == -1 && errno != 0), "sched_get_priority_min(SCHED_OTHER) failed: %s", pc_errname(errno));
	errno = 0;
	hi = sched_get_priority_max(SCHED_OTHER);
	pc_check(!(hi == -1 && errno != 0), "sched_get_priority_max(SCHED_OTHER) failed: %s", pc_errname(errno));
	pc_check(lo <= hi, "minimum %d is above maximum %d", lo, hi);
	pc_pass();
}
