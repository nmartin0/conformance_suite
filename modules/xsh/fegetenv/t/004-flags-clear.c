/*
 * fesetenv() reinstates exception flags that were clear when stored.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fegetenv.004
 */
#include "fh.h"

int main(void)
{
	fenv_t env;
	int e;

	pc_begin("XSH.fegetenv.004");
	if (FH_NEXC == 0)
		pc_notinuse("no exception macro is defined");
	e = fh_exc[0];
	fh_clearall();
	if (fegetenv(&env) != 0)
		pc_unresolved("fegetenv() failed");
	fh_raise(e);
	pc_test();
	if (fesetenv(&env) != 0)
		pc_fail("fesetenv() failed");
	pc_check((fetestexcept(e) & e) == 0, "exception 0x%x is still set after fesetenv() with a stored clear flag", e);
	pc_pass();
}
