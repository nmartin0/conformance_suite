/*
 * pthread_setschedparam() on the calling thread; pthread_getschedparam()
 * reads back the policy and priority.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_getschedparam.003
 */
#include "schedh.h"

static int body(char *msg)
{
	pthread_t me = pthread_self();
	int lo, hi, rlo, rhi, olo, r, e, pol, prio;

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
	if ((r = sh_tenter(me, SCHED_FIFO, lo + 1, msg)) != SH_PASS)
		return r;
	if ((r = sh_texpect(me, SCHED_FIFO, lo + 1, msg)) != SH_PASS)
		return r;
	if ((e = sh_tset(me, SCHED_RR, rhi)) != 0) {
		SH_SAY(msg, "pthread_setschedparam(RR, %d) failed: %s", rhi, pc_errname(e));
		return SH_FAIL;
	}
	if ((r = sh_texpect(me, SCHED_RR, rhi, msg)) != SH_PASS)
		return r;
	if ((e = sh_tset(me, SCHED_FIFO, lo)) != 0) {
		SH_SAY(msg, "pthread_setschedparam(FIFO, %d) failed: %s", lo, pc_errname(e));
		return SH_FAIL;
	}
	if ((r = sh_texpect(me, SCHED_FIFO, lo, msg)) != SH_PASS)
		return r;
	if ((e = sh_tset(me, SCHED_OTHER, olo)) != 0) {
		SH_SAY(msg, "pthread_setschedparam(OTHER, %d) failed: %s", olo, pc_errname(e));
		return SH_FAIL;
	}
	e = sh_tget(me, &pol, &prio);
	if (e != 0 || pol != SCHED_OTHER) {
		SH_SAY(msg, "after SCHED_OTHER the policy is %d (error %s)", pol, pc_errname(e));
		return SH_FAIL;
	}
	return SH_PASS;
}

int main(void)
{
	pc_begin("XSH.pthread_getschedparam.003");
	pc_test();
	sh_run(body, 0);
	pc_pass();
}
