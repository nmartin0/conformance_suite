/*
 * pthread_setschedprio() with a priority outside the range fails and
 * changes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_setschedprio.002
 */
#include "schedh.h"

static int body(char *msg)
{
	pthread_t me = pthread_self();
	int lo, hi, r, e, i, v[2], p2, q2;

	if ((r = sh_rng(SCHED_FIFO, &lo, &hi, msg)) != SH_PASS)
		return r;
	if ((r = sh_tenter(me, SCHED_FIFO, lo + 1, msg)) != SH_PASS)
		return r;
	v[0] = hi + 1;
	v[1] = lo - 1;
	for (i = 0; i < 2; i++) {
		e = pthread_setschedprio(me, v[i]);
		if (e == 0) {
			/* EINVAL is only a may-fail error, but the thread must not
			 * be left with a priority outside its policy's range. */
			if (sh_tget(me, &p2, &q2) != 0 || q2 < sched_get_priority_min(p2) ||
			    q2 > sched_get_priority_max(p2)) {
				SH_SAY(msg, "priority %d was accepted and left the thread outside its range", v[i]);
				return SH_FAIL;
			}
			(void)pthread_setschedprio(me, lo + 1);
			continue;
		}
		if (e != EINVAL && e != EPERM) {
			SH_SAY(msg, "priority %d gave %s, expected EINVAL or EPERM", v[i], pc_errname(e));
			return SH_FAIL;
		}
		if ((r = sh_texpect(me, SCHED_FIFO, lo + 1, msg)) != SH_PASS)
			return r;
	}
	return SH_PASS;
}

int main(void)
{
	pc_begin("XSH.pthread_setschedprio.002");
	pc_test();
	sh_run(body, 0);
	pc_pass();
}
