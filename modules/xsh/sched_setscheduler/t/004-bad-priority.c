/*
 * sched_setscheduler() with a priority outside the new policy's range
 * fails with EINVAL and changes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_setscheduler.004
 */
#include "schedh.h"

static int body(char *msg)
{
	struct sched_param sp;
	int lo, hi, rlo, rhi, r, i, pol[3], pri[3];

	if ((r = sh_rng(SCHED_FIFO, &lo, &hi, msg)) != SH_PASS)
		return r;
	if ((r = sh_rng(SCHED_RR, &rlo, &rhi, msg)) != SH_PASS)
		return r;
	if ((r = sh_enter(SCHED_FIFO, lo + 1, msg)) != SH_PASS)
		return r;
	pol[0] = SCHED_RR;
	pri[0] = rhi + 1;
	pol[1] = SCHED_RR;
	pri[1] = rlo - 1;
	pol[2] = SCHED_FIFO;
	pri[2] = hi + 1;
	for (i = 0; i < 3; i++) {
		memset(&sp, 0, sizeof sp);
		sp.sched_priority = pri[i];
		errno = 0;
		if (sched_setscheduler(0, pol[i], &sp) != -1 || errno != EINVAL) {
			SH_SAY(msg, "policy %d priority %d did not give -1/EINVAL (errno %s)", pol[i], pri[i], pc_errname(errno));
			return SH_FAIL;
		}
		if ((r = sh_expect(SCHED_FIFO, lo + 1, msg)) != SH_PASS)
			return r;
	}
	return SH_PASS;
}

int main(void)
{
	pc_begin("XSH.sched_setscheduler.004");
	pc_test();
	sh_run(body, 0);
	pc_pass();
}
