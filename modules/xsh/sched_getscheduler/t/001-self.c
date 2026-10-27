/*
 * sched_getscheduler(0) returns a policy, the same as for the caller's pid.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_getscheduler.001
 */
#include "pc.h"
#include <errno.h>
#include <sched.h>
#include <unistd.h>

int main(void)
{
	int a, b;

	pc_begin("XSH.sched_getscheduler.001");
	pc_test();
	errno = 0;
	a = sched_getscheduler(0);
	pc_check(a != -1, "sched_getscheduler(0) failed: %s", pc_errname(errno));
	pc_check(a >= 0, "sched_getscheduler(0) returned the negative value %d", a);
	errno = 0;
	b = sched_getscheduler(getpid());
	pc_check(b != -1, "sched_getscheduler(getpid()) failed: %s", pc_errname(errno));
	pc_check(a == b, "pid 0 gives policy %d, the caller's own pid gives %d", a, b);
	pc_pass();
}
