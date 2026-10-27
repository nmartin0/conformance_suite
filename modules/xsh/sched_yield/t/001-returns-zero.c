/*
 * sched_yield() returns 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_yield.001
 */
#include "pc.h"
#include <errno.h>
#include <sched.h>

int main(void)
{
	int i;

	pc_begin("XSH.sched_yield.001");
	pc_test();
	for (i = 0; i < 100; i++) {
		errno = 0;
		pc_check(sched_yield() == 0, "call %d did not return 0 (errno %s)", i, pc_errname(errno));
	}
	pc_pass();
}
