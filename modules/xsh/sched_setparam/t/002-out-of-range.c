/*
 * sched_setparam() with a priority outside the range fails with EINVAL and
 * leaves the priority as it was.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_setparam.002
 */
#include "schedh.h"

static int body(char *msg)
{
	struct sched_param sp;
	int lo, hi, r, v[2], i;

	if ((r = sh_rng(SCHED_FIFO, &lo, &hi, msg)) != SH_PASS)
		return r;
	if ((r = sh_enter(SCHED_FIFO, lo + 1, msg)) != SH_PASS)
		return r;
	v[0] = hi + 1;
	v[1] = lo - 1;
	for (i = 0; i < 2; i++) {
		memset(&sp, 0, sizeof sp);
		sp.sched_priority = v[i];
		errno = 0;
		if (sched_setparam(0, &sp) != -1 || errno != EINVAL) {
			SH_SAY(msg, "sched_setparam(0, %d) did not fail with EINVAL (errno %s)", v[i], pc_errname(errno));
			return SH_FAIL;
		}
		if ((r = sh_expect(SCHED_FIFO, lo + 1, msg)) != SH_PASS)
			return r;
	}
	return SH_PASS;
}

int main(void)
{
	pc_begin("XSH.sched_setparam.002");
	pc_test();
	sh_run(body, 0);
	pc_pass();
}
