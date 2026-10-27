/*
 * feclearexcept() clears the exception it is given.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feclearexcept.002
 */
#include "fh.h"

int main(void)
{
	int i, e;

	pc_begin("XSH.feclearexcept.002");
	if (FH_NEXC == 0)
		pc_notinuse("no exception macro is defined");
	pc_test();
	for (i = 0; i < FH_NEXC; i++) {
		e = fh_exc[i];
		fh_raise(e);
		(void)feclearexcept(e);
		pc_check((fetestexcept(e) & e) == 0, "exception 0x%x still set after feclearexcept()", e);
		fh_clearall();
	}
	pc_pass();
}
