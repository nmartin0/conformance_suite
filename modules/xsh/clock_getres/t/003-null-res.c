/*
 * clock_getres() with a null res succeeds.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock_getres.003
 * MUTANT    clock_getres.null_efault
 */
#include "pc.h"
#include <errno.h>
#include <time.h>

int main(void)
{
	int r;

	pc_begin("XSH.clock_getres.003");
	pc_test();
	r = clock_getres(CLOCK_REALTIME, NULL);
	pc_check(r == 0, "clock_getres(CLOCK_REALTIME, NULL) returned %d (%s)", r, pc_errname(errno));
	r = clock_getres(CLOCK_MONOTONIC, NULL);
	pc_check(r == 0, "clock_getres(CLOCK_MONOTONIC, NULL) returned %d (%s)", r, pc_errname(errno));
	pc_pass();
}
