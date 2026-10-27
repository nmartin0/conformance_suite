/*
 * sched_setparam() of a pid no process has fails with ESRCH.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_setparam.003
 */
#include "schedh.h"

int main(void)
{
	struct sched_param sp;
	pid_t dead;

	pc_begin("XSH.sched_setparam.003");
	dead = sh_dead_pid();
	if (sched_getparam(0, &sp) != 0)
		pc_unresolved("sched_getparam(0) failed: %s", pc_errname(errno));
	pc_test();
	errno = 0;
	pc_expect_err(sched_setparam(dead, &sp), ESRCH, "sched_setparam(reaped child)");
	pc_pass();
}
