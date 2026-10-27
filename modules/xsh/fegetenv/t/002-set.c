/*
 * fesetenv() accepts an environment stored by fegetenv().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetenv.002
 */
#include "fh.h"

int main(void)
{
	fenv_t env;
	int r;

	pc_begin("XSH.fegetenv.002");
	if (fegetenv(&env) != 0)
		pc_unresolved("fegetenv() failed");
	pc_test();
	r = fesetenv(&env);
	pc_check(r == 0, "fesetenv() returned %d, expected 0", r);
	pc_pass();
}
