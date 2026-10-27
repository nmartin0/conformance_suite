/*
 * feholdexcept() returns zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feholdexcept.001
 */
#include "fh.h"

int main(void)
{
	fenv_t env;
	int r;

	pc_begin("XSH.feholdexcept.001");
	pc_test();
	r = feholdexcept(&env);
	(void)fesetenv(&env);
	if (r != 0)
		pc_unresolved("feholdexcept() returned %d: the page does not say when a non-stop mode must be available", r);
	pc_pass();
}
