/*
 * Fixture for ci/mutate.sh: two calls of getppid() agree (disputed).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pcfx.002
 * MUTANT    pcfx.pid_zero
 * READING   pcfx.changes
 */
#include "pc.h"
#include <unistd.h>

int main(void)
{
	pid_t a, b;

	pc_begin("XSH.pcfx.002");
	pc_test();
	a = getppid();
	b = getppid();
	pc_check(a > 0 && a == b, "getppid() gave %ld then %ld", (long)a, (long)b);
	pc_pass();
}
