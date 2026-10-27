/*
 * pthread_setschedparam() with a priority outside the range fails and
 * changes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_getschedparam.005
 */
#include "schedh.h"

static int body(char *msg)
{
	pthread_t me = pthread_self();
	int lo, hi, rlo, rhi, r, e, i, pol[3], pri[3], p2, q2;

	if ((r = sh_rng(SCHED_FIFO, &lo, &hi, msg)) != SH_PASS)
		return r;
	if ((r = sh_rng(SCHED_RR, &rlo, &rhi, msg)) != SH_PASS)
		return r;
	if ((r = sh_tenter(me, SCHED_FIFO, lo + 1, msg)) != SH_PASS)
		return r;
	pol[0] = SCHED_FIFO;
	pri[0] = hi + 1;
	pol[1] = SCHED_RR;
	pri[1] = rlo - 1;
	pol[2] = SCHED_RR;
	pri[2] = rhi + 1;
	for (i = 0; i < 3; i++) {
		e = sh_tset(me, pol[i], pri[i]);
		if (e == 0) {
			/* Success is not forbidden outright, but the thread must
			 * not be left with a priority outside its policy's range. */
			if (sh_tget(me, &p2, &q2) != 0 || q2 < sched_get_priority_min(p2) ||
			    q2 > sched_get_priority_max(p2)) {
				SH_SAY(msg, "policy %d priority %d was accepted and left the thread outside its range", pol[i], pri[i]);
				return SH_FAIL;
			}
			(void)sh_tenter(me, SCHED_FIFO, lo + 1, msg);
			continue;
		}
		if (e != EINVAL && e != ENOTSUP && e != EPERM) {
			SH_SAY(msg, "policy %d priority %d gave %s, expected EINVAL, ENOTSUP or EPERM", pol[i], pri[i], pc_errname(e));
			return SH_FAIL;
		}
		if ((r = sh_texpect(me, SCHED_FIFO, lo + 1, msg)) != SH_PASS)
			return r;
	}
	return SH_PASS;
}

int main(void)
{
	pc_begin("XSH.pthread_getschedparam.005");
	pc_test();
	sh_run(body, 0);
	pc_pass();
}
