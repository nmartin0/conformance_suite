/*
 * sched_setscheduler() of a pid no process has fails with ESRCH.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_setscheduler.005
 */
#include "schedh.h"

int main(void)
{
	struct sched_param sp;
	pid_t dead;
	int pol;

	pc_begin("XSH.sched_setscheduler.005");
	dead = sh_dead_pid();
	pol = sched_getscheduler(0);
	if (pol == -1 || sched_getparam(0, &sp) != 0)
		pc_unresolved("cannot read the caller's own policy: %s", pc_errname(errno));
	pc_test();
	errno = 0;
	pc_expect_err(sched_setscheduler(dead, pol, &sp), ESRCH, "sched_setscheduler(reaped child)");
	pc_pass();
}
