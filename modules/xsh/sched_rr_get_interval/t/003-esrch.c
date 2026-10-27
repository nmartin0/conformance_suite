/*
 * sched_rr_get_interval() of a pid no process has fails with ESRCH.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_rr_get_interval.003
 */
#include "schedh.h"

int main(void)
{
	struct timespec ts;
	pid_t dead;

	pc_begin("XSH.sched_rr_get_interval.003");
	dead = sh_dead_pid();
	pc_test();
	errno = 0;
	pc_expect_err(sched_rr_get_interval(dead, &ts), ESRCH, "sched_rr_get_interval(reaped child)");
	pc_pass();
}
