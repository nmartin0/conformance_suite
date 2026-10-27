/*
 * feupdateenv() raises again the exceptions that were raised when it was called.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feupdateenv.005
 */
#include "fh.h"

int main(void)
{
	fenv_t env;
	int e;

	pc_begin("XSH.feupdateenv.005");
	if (FH_NEXC == 0)
		pc_notinuse("no exception macro is defined");
	e = fh_exc[0];
	fh_clearall();
	if (fegetenv(&env) != 0)	/* stored with every flag clear */
		pc_unresolved("fegetenv() failed");
	fh_raise(e);
	pc_test();
	if (feupdateenv(&env) != 0)
		pc_fail("feupdateenv() failed");
	pc_check((fetestexcept(e) & e) != 0, "exception 0x%x, raised before feupdateenv(), is not set after it", e);
	fh_clearall();
	pc_pass();
}
