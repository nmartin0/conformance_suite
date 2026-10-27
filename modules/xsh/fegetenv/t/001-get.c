/*
 * fegetenv() stores the environment and returns zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetenv.001
 */
#include "fh.h"

int main(void)
{
	fenv_t env;
	int r;

	pc_begin("XSH.fegetenv.001");
	pc_test();
	r = fegetenv(&env);
	pc_check(r == 0, "fegetenv() returned %d, expected 0", r);
	pc_pass();
}
