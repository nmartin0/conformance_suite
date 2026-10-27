/*
 * feupdateenv() returns zero for an environment stored by feholdexcept().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feupdateenv.002
 */
#include "fh.h"

int main(void)
{
	fenv_t env;
	int r;

	pc_begin("XSH.feupdateenv.002");
	if (feholdexcept(&env) != 0)
		pc_unresolved("feholdexcept() did not return zero");
	pc_test();
	r = feupdateenv(&env);
	pc_check(r == 0, "feupdateenv() returned %d, expected 0", r);
	fh_clearall();
	pc_pass();
}
