/*
 * sched_rr_get_interval(0) succeeds and stores a valid time.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_rr_get_interval.002
 */
#include "schedh.h"

int main(void)
{
	struct timespec ts;

	pc_begin("XSH.sched_rr_get_interval.002");
	ts.tv_sec = -7;
	ts.tv_nsec = -7;
	pc_test();
	pc_expect_ok(sched_rr_get_interval(0, &ts), "sched_rr_get_interval(0)");
	pc_check(ts.tv_sec >= 0 && ts.tv_nsec >= 0 && ts.tv_nsec < 1000000000L,
	    "interval %ld s %ld ns is not a valid time", (long)ts.tv_sec, (long)ts.tv_nsec);
	pc_pass();
}
