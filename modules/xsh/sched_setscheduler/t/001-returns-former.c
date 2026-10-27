/*
 * sched_setscheduler() returns the policy that was in force before.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_setscheduler.001
 */
#include "schedh.h"

static int set(int pol, int prio, int want, char *msg)
{
	struct sched_param sp;
	int r;

	memset(&sp, 0, sizeof sp);
	sp.sched_priority = prio;
	r = sched_setscheduler(0, pol, &sp);
	if (r == -1) {
		SH_SAY(msg, "sched_setscheduler(policy %d, priority %d) failed: %s", pol, prio, pc_errname(errno));
		return errno == EPERM ? SH_UNRES : SH_FAIL;
	}
	if (r != want) {
		SH_SAY(msg, "returned %d, the former policy was %d", r, want);
		return SH_FAIL;
	}
	return SH_PASS;
}

static int body(char *msg)
{
	int lo, hi, rlo, rhi, olo, r, prev;

	if ((r = sh_rng(SCHED_FIFO, &lo, &hi, msg)) != SH_PASS)
		return r;
	if ((r = sh_rng(SCHED_RR, &rlo, &rhi, msg)) != SH_PASS)
		return r;
	errno = 0;
	olo = sched_get_priority_min(SCHED_OTHER);
	if (olo == -1 && errno != 0) {
		SH_SAY(msg, "no SCHED_OTHER range: %s", pc_errname(errno));
		return SH_UNRES;
	}
	prev = sched_getscheduler(0);
	if (prev == -1) {
		SH_SAY(msg, "sched_getscheduler(0) failed: %s", pc_errname(errno));
		return SH_UNRES;
	}
	if ((r = sh_enter(prev == SCHED_FIFO ? SCHED_RR : SCHED_FIFO, lo, msg)) != SH_PASS)
		return r;
	prev = prev == SCHED_FIFO ? SCHED_RR : SCHED_FIFO;
	if ((r = set(SCHED_FIFO, lo + 1, prev, msg)) != SH_PASS)
		return r;
	if ((r = set(SCHED_RR, rhi, SCHED_FIFO, msg)) != SH_PASS)
		return r;
	return set(SCHED_OTHER, olo, SCHED_RR, msg);
}

int main(void)
{
	pc_begin("XSH.sched_setscheduler.001");
	pc_test();
	sh_run(body, 0);
	pc_pass();
}
