/*
 * Fixture for ci/mutate.sh: never builds here, so its pairs are UNCHECKED.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pcfx.003
 * MUTANT    pcfx.pid_zero
 * VARIANT   pcfx.pid_same
 */
#include "pc.h"

int main(void)
{
	pc_begin("XSH.pcfx.003");
	pc_test();
	pc_check(pcfx_no_such_function() == 0, "unreachable");
	pc_pass();
}
