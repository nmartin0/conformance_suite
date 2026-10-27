/*
 * fetestexcept() returns the macro of an exception that an operation raised.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fetestexcept.002
 */
#include "fh.h"

int main(void)
{
	int i, e, r;

	pc_begin("XSH.fetestexcept.002");
	if (FH_NEXC == 0)
		pc_notinuse("no exception macro is defined");
	pc_test();
	for (i = 0; i < FH_NEXC; i++) {
		e = fh_exc[i];
		fh_clearall();
		fh_raise(e);
		r = fetestexcept(e);
		pc_check(r == e, "fetestexcept(0x%x) returned 0x%x after the exception was raised", e, r);
	}
	fh_clearall();
	pc_pass();
}
