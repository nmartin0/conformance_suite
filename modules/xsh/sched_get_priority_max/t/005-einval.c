/*
 * Both functions refuse a value that is no policy with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_get_priority_max.005
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <sched.h>

int main(void)
{
	int bad = INT_MAX;

	pc_begin("XSH.sched_get_priority_max.005");
	while (bad == SCHED_FIFO || bad == SCHED_RR || bad == SCHED_OTHER
#ifdef SCHED_SPORADIC
	       || bad == SCHED_SPORADIC
#endif
	       )
		bad--;
	pc_test();
	errno = 0;
	pc_expect_err(sched_get_priority_max(bad), EINVAL, "sched_get_priority_max(undefined policy)");
	errno = 0;
	pc_expect_err(sched_get_priority_min(bad), EINVAL, "sched_get_priority_min(undefined policy)");
	errno = 0;
	pc_expect_err(sched_get_priority_max(INT_MIN), EINVAL, "sched_get_priority_max(INT_MIN)");
	errno = 0;
	pc_expect_err(sched_get_priority_min(INT_MIN), EINVAL, "sched_get_priority_min(INT_MIN)");
	pc_pass();
}
