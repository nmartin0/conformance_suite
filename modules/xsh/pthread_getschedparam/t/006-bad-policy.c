/*
 * pthread_setschedparam() with a policy that does not exist fails and
 * changes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_getschedparam.006
 */
#include "schedh.h"

static int body(char *msg)
{
	pthread_t me = pthread_self();
	int lo, hi, r, e;

	if ((r = sh_rng(SCHED_FIFO, &lo, &hi, msg)) != SH_PASS)
		return r;
	if ((r = sh_tenter(me, SCHED_FIFO, lo + 1, msg)) != SH_PASS)
		return r;
	e = sh_tset(me, sh_bad_policy(), lo + 1);
	if (e != EINVAL && e != ENOTSUP && e != EPERM) {
		SH_SAY(msg, "an undefined policy gave %s, expected EINVAL, ENOTSUP or EPERM", e == 0 ? "success" : pc_errname(e));
		return SH_FAIL;
	}
	return sh_texpect(me, SCHED_FIFO, lo + 1, msg);
}

int main(void)
{
	pc_begin("XSH.pthread_getschedparam.006");
	pc_test();
	sh_run(body, 0);
	pc_pass();
}
