/*
 * sched_setscheduler() with a policy that does not exist fails with EINVAL
 * and changes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_setscheduler.003
 */
#include "schedh.h"

static int body(char *msg)
{
	struct sched_param sp;
	int lo, hi, r;

	if ((r = sh_rng(SCHED_FIFO, &lo, &hi, msg)) != SH_PASS)
		return r;
	if ((r = sh_enter(SCHED_FIFO, lo + 1, msg)) != SH_PASS)
		return r;
	memset(&sp, 0, sizeof sp);
	sp.sched_priority = lo + 1;
	errno = 0;
	if (sched_setscheduler(0, sh_bad_policy(), &sp) != -1 || errno != EINVAL) {
		SH_SAY(msg, "an undefined policy did not give -1/EINVAL (errno %s)", pc_errname(errno));
		return SH_FAIL;
	}
	return sh_expect(SCHED_FIFO, lo + 1, msg);
}

int main(void)
{
	pc_begin("XSH.sched_setscheduler.003");
	pc_test();
	sh_run(body, 0);
	pc_pass();
}
