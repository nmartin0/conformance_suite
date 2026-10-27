/*
 * feholdexcept() clears the exception flags.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feholdexcept.003
 */
#include "fh.h"

int main(void)
{
	fenv_t env;
	int i, r;

	pc_begin("XSH.feholdexcept.003");
	if (FH_NEXC == 0)
		pc_notinuse("no exception macro is defined");
	fh_clearall();
	for (i = 0; i < FH_NEXC; i++)
		fh_raise(fh_exc[i]);
	pc_test();
	if (feholdexcept(&env) != 0)
		pc_unresolved("feholdexcept() did not return zero");
	r = fetestexcept(FE_ALL_EXCEPT);
	pc_check(r == 0, "flags 0x%x are set after feholdexcept()", r);
	(void)fesetenv(&env);
	fh_clearall();
	pc_pass();
}
