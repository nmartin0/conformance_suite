/*
 * Without privilege, pthread_setschedparam() to SCHED_FIFO either works or
 * fails with EPERM or ENOTSUP and leaves the thread alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_getschedparam.008
 */
#include "schedh.h"

static int body(char *msg)
{
	pthread_t me = pthread_self();
	int lo, hi, r, e, pol0, prio0;

	if ((r = sh_rng(SCHED_FIFO, &lo, &hi, msg)) != SH_PASS)
		return r;
	if ((e = sh_tget(me, &pol0, &prio0)) != 0) {
		SH_SAY(msg, "pthread_getschedparam failed: %s", pc_errname(e));
		return SH_UNRES;
	}
	e = sh_tset(me, SCHED_FIFO, lo);
	if (e == 0)
		return sh_texpect(me, SCHED_FIFO, lo, msg);
	if (e != EPERM && e != ENOTSUP) {
		SH_SAY(msg, "failed with %s, only EPERM or ENOTSUP apply", pc_errname(e));
		return SH_FAIL;
	}
	return sh_texpect(me, pol0, prio0, msg);
}

int main(void)
{
	pc_begin("XSH.pthread_getschedparam.008");
	pc_test();
	sh_run(body, 1);
	pc_pass();
}
