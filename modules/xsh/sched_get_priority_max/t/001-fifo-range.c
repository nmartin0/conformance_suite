/*
 * The SCHED_FIFO priority range is non-empty and has at least 32 values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_get_priority_max.001
 */
#include "pc.h"
#include <errno.h>
#include <sched.h>

int main(void)
{
	int lo, hi;

	pc_begin("XSH.sched_get_priority_max.001");
	pc_test();
	errno = 0;
	lo = sched_get_priority_min(SCHED_FIFO);
	pc_check(!(lo == -1 && errno != 0), "sched_get_priority_min(SCHED_FIFO) failed: %s", pc_errname(errno));
	errno = 0;
	hi = sched_get_priority_max(SCHED_FIFO);
	pc_check(!(hi == -1 && errno != 0), "sched_get_priority_max(SCHED_FIFO) failed: %s", pc_errname(errno));
	pc_check(lo < hi, "minimum %d is not below maximum %d", lo, hi);
	pc_check((long)hi - lo + 1 >= 32, "only %ld priorities (%d..%d)", (long)hi - lo + 1, lo, hi);
	pc_pass();
}
