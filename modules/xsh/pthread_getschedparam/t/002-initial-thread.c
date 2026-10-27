/*
 * The initial thread has the policy and priority of its process.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_getschedparam.002
 */
#include "schedh.h"

int main(void)
{
	struct sched_param sp;
	int pol, prio, ppol, e;

	pc_begin("XSH.pthread_getschedparam.002");
	ppol = sched_getscheduler(0);
	memset(&sp, 0, sizeof sp);
	if (ppol == -1 || sched_getparam(0, &sp) != 0)
		pc_unresolved("cannot read the process's policy: %s", pc_errname(errno));
	pc_test();
	e = sh_tget(pthread_self(), &pol, &prio);
	pc_check(e == 0, "pthread_getschedparam failed: %s", pc_errname(e));
	pc_check(pol == ppol, "thread policy %d differs from the process's %d", pol, ppol);
	pc_check(prio == sp.sched_priority, "thread priority %d differs from the process's %d", prio, sp.sched_priority);
	pc_pass();
}
