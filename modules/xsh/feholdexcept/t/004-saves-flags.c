/*
 * The environment saved by feholdexcept() holds the flags set before the call.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feholdexcept.004
 */
#include "fh.h"

int main(void)
{
	fenv_t env;
	int e;

	pc_begin("XSH.feholdexcept.004");
	if (FH_NEXC == 0)
		pc_notinuse("no exception macro is defined");
	e = fh_exc[0];
	fh_clearall();
	fh_raise(e);
	if (feholdexcept(&env) != 0)
		pc_unresolved("feholdexcept() did not return zero");
	if ((fetestexcept(e) & e) != 0)
		pc_unresolved("exception 0x%x not cleared by feholdexcept()", e);
	pc_test();
	if (fesetenv(&env) != 0)
		pc_fail("fesetenv() failed");
	pc_check((fetestexcept(e) & e) != 0, "exception 0x%x, set before feholdexcept(), is not set after fesetenv() with the held environment", e);
	fh_clearall();
	pc_pass();
}
