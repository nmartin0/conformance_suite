/*
 * feupdateenv() returns zero for an environment stored by fegetenv().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feupdateenv.001
 */
#include "fh.h"

int main(void)
{
	fenv_t env;
	int r;

	pc_begin("XSH.feupdateenv.001");
	if (fegetenv(&env) != 0)
		pc_unresolved("fegetenv() failed");
	pc_test();
	r = feupdateenv(&env);
	pc_check(r == 0, "feupdateenv() returned %d, expected 0", r);
	fh_clearall();
	pc_pass();
}
