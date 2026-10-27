/*
 * sched_setparam(0) changes the priority; sched_getparam(0) reads it back.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_setparam.001
 */
#include "schedh.h"

static int body(char *msg)
{
	struct sched_param sp;
	int lo, hi, r, v[3], i;

	if ((r = sh_rng(SCHED_FIFO, &lo, &hi, msg)) != SH_PASS)
		return r;
	if ((r = sh_enter(SCHED_FIFO, lo, msg)) != SH_PASS)
		return r;
	v[0] = lo + 1;
	v[1] = hi;
	v[2] = lo;
	for (i = 0; i < 3; i++) {
		memset(&sp, 0, sizeof sp);
		sp.sched_priority = v[i];
		if (sched_setparam(0, &sp) != 0) {
			SH_SAY(msg, "sched_setparam(0, %d) failed: %s", v[i], pc_errname(errno));
			return SH_FAIL;
		}
		sp.sched_priority = INT_MIN;
		if (sched_getparam(0, &sp) != 0) {
			SH_SAY(msg, "sched_getparam failed: %s", pc_errname(errno));
			return SH_FAIL;
		}
		if (sp.sched_priority != v[i]) {
			SH_SAY(msg, "set %d, read back %d", v[i], sp.sched_priority);
			return SH_FAIL;
		}
	}
	return SH_PASS;
}

int main(void)
{
	pc_begin("XSH.sched_setparam.001");
	pc_test();
	sh_run(body, 0);
	pc_pass();
}
