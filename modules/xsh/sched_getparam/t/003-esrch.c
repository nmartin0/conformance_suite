/*
 * sched_getparam() of a pid no process has fails with ESRCH.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_getparam.003
 */
#include "schedh.h"

int main(void)
{
	struct sched_param sp;
	pid_t dead;

	pc_begin("XSH.sched_getparam.003");
	dead = sh_dead_pid();
	memset(&sp, 0, sizeof sp);
	pc_test();
	errno = 0;
	pc_expect_err(sched_getparam(dead, &sp), ESRCH, "sched_getparam(reaped child)");
	pc_pass();
}
