/*
 * Under SCHED_RR, sched_rr_get_interval(0) updates the interval.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_rr_get_interval.001
 */
#include "schedh.h"

static int body(char *msg)
{
	struct timespec ts;
	int lo, hi, r;

	if ((r = sh_rng(SCHED_RR, &lo, &hi, msg)) != SH_PASS)
		return r;
	if ((r = sh_enter(SCHED_RR, lo, msg)) != SH_PASS)
		return r;
	ts.tv_sec = -7;
	ts.tv_nsec = -7;
	if (sched_rr_get_interval(0, &ts) != 0) {
		SH_SAY(msg, "sched_rr_get_interval(0) failed: %s", pc_errname(errno));
		return SH_FAIL;
	}
	if (ts.tv_sec < 0 || ts.tv_nsec < 0 || ts.tv_nsec >= 1000000000L) {
		SH_SAY(msg, "interval %ld s %ld ns is not a valid time", (long)ts.tv_sec, (long)ts.tv_nsec);
		return SH_FAIL;
	}
	return SH_PASS;
}

int main(void)
{
	pc_begin("XSH.sched_rr_get_interval.001");
	pc_test();
	sh_run(body, 0);
	pc_pass();
}
