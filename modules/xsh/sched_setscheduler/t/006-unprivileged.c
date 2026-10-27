/*
 * Without privilege, a change to SCHED_FIFO either works or fails with
 * EPERM; a failure leaves policy and priority alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_setscheduler.006
 */
#include "schedh.h"

static int body(char *msg)
{
	struct sched_param sp;
	int lo, hi, r, pol0, prio0, got, saved;

	if ((r = sh_rng(SCHED_FIFO, &lo, &hi, msg)) != SH_PASS)
		return r;
	pol0 = sched_getscheduler(0);
	memset(&sp, 0, sizeof sp);
	if (pol0 == -1 || sched_getparam(0, &sp) != 0) {
		SH_SAY(msg, "cannot read the policy: %s", pc_errname(errno));
		return SH_UNRES;
	}
	prio0 = sp.sched_priority;
	sp.sched_priority = lo;
	errno = 0;
	got = sched_setscheduler(0, SCHED_FIFO, &sp);
	saved = errno;
	if (got == -1) {
		if (saved != EPERM) {
			SH_SAY(msg, "failed with %s, only EPERM applies", pc_errname(saved));
			return SH_FAIL;
		}
		return sh_expect(pol0, prio0, msg);
	}
	if (got != pol0) {
		SH_SAY(msg, "returned %d, the former policy was %d", got, pol0);
		return SH_FAIL;
	}
	return sh_expect(SCHED_FIFO, lo, msg);
}

int main(void)
{
	pc_begin("XSH.sched_setscheduler.006");
	pc_test();
	sh_run(body, 1);
	pc_pass();
}
