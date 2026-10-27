/*
 * sched_getscheduler() of a pid no process has fails with ESRCH.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_getscheduler.002
 */
#include "schedh.h"

int main(void)
{
	pid_t dead;

	pc_begin("XSH.sched_getscheduler.002");
	dead = sh_dead_pid();
	pc_test();
	errno = 0;
	pc_expect_err(sched_getscheduler(dead), ESRCH, "sched_getscheduler(reaped child)");
	pc_pass();
}
