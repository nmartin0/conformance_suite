/*
 * pthread_setschedprio() changes the priority and keeps the policy.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_setschedprio.001
 */
#include "schedh.h"

static int body(char *msg)
{
	pthread_t me = pthread_self();
	int lo, hi, r, e, i, v[4];

	if ((r = sh_rng(SCHED_FIFO, &lo, &hi, msg)) != SH_PASS)
		return r;
	if ((r = sh_tenter(me, SCHED_FIFO, lo + 1, msg)) != SH_PASS)
		return r;
	v[0] = lo + 3;
	v[1] = hi;
	v[2] = lo;
	v[3] = lo + 2;
	for (i = 0; i < 4; i++) {
		if ((e = pthread_setschedprio(me, v[i])) != 0) {
			SH_SAY(msg, "pthread_setschedprio(%d) failed: %s", v[i], pc_errname(e));
			return SH_FAIL;
		}
		if ((r = sh_texpect(me, SCHED_FIFO, v[i], msg)) != SH_PASS)
			return r;
	}
	return SH_PASS;
}

int main(void)
{
	pc_begin("XSH.pthread_setschedprio.001");
	pc_test();
	sh_run(body, 0);
	pc_pass();
}
