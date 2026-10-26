/*
 * Fixture for ci/mutate.sh: getppid() is positive and differs from getpid().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pcfx.001
 * MUTANT    pcfx.pid_zero
 * MUTANT    pcfx.harmless
 * MUTANT    pcfx.sometimes
 * VARIANT   pcfx.pid_same
 * VARIANT   pcfx.pid_neg
 * VARIANT   pcfx.ppid_bad
 */
#include "pc.h"
#include <unistd.h>

int main(void)
{
	pid_t p, q;

	pc_begin("XSH.pcfx.001");
	if (getppid() <= 0)
		pc_unresolved("getppid() failed in setup");
	pc_test();
	p = getppid();
	q = getpid();
	pc_check(p > 0, "getppid() returned %ld", (long)p);
	pc_check(p != q, "getppid() equals getpid()");
	pc_pass();
}
