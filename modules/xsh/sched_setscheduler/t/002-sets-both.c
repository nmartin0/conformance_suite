/*
 * sched_setscheduler() sets the policy and the priority.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_setscheduler.002
 */
#include "schedh.h"

static int body(char *msg)
{
	int lo, hi, rlo, rhi, r;

	if ((r = sh_rng(SCHED_FIFO, &lo, &hi, msg)) != SH_PASS)
		return r;
	if ((r = sh_rng(SCHED_RR, &rlo, &rhi, msg)) != SH_PASS)
		return r;
	if ((r = sh_enter(SCHED_FIFO, lo + 1, msg)) != SH_PASS)
		return r;
	if ((r = sh_expect(SCHED_FIFO, lo + 1, msg)) != SH_PASS)
		return r;
	if ((r = sh_enter(SCHED_RR, rhi, msg)) != SH_PASS)
		return r;
	if ((r = sh_expect(SCHED_RR, rhi, msg)) != SH_PASS)
		return r;
	if ((r = sh_enter(SCHED_FIFO, lo, msg)) != SH_PASS)
		return r;
	return sh_expect(SCHED_FIFO, lo, msg);
}

int main(void)
{
	pc_begin("XSH.sched_setscheduler.002");
	pc_test();
	sh_run(body, 0);
	pc_pass();
}
