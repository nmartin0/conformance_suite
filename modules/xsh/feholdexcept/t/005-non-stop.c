/*
 * After feholdexcept() an exceptional operation raises its flag and execution continues.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feholdexcept.005
 */
#include "fh.h"

int main(void)
{
	fenv_t env;
	int i, e;

	pc_begin("XSH.feholdexcept.005");
	if (FH_NEXC == 0)
		pc_notinuse("no exception macro is defined");
	for (i = 0; i < FH_NEXC; i++)	/* the arithmetic must raise each flag at all */
		fh_raise(fh_exc[i]);
	if (feholdexcept(&env) != 0)
		pc_unresolved("feholdexcept() did not return zero");
	pc_test();
	for (i = 0; i < FH_NEXC; i++) {
		e = fh_exc[i];
		if (!fh_arith(e))
			pc_unresolved("no arithmetic for exception 0x%x", e);
		pc_check((fetestexcept(e) & e) != 0, "exception 0x%x was not recorded in the flags after an exceptional operation", e);
		fh_clearall();
	}
	(void)fesetenv(&env);
	pc_pass();
}
