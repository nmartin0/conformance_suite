/*
 * feraiseexcept(FE_ALL_EXCEPT) returns zero and sets every defined flag.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feraiseexcept.004
 */
#include "fh.h"

int main(void)
{
	int i, e, r;

	pc_begin("XSH.feraiseexcept.004");
	if (FE_ALL_EXCEPT == 0)
		pc_notinuse("FE_ALL_EXCEPT is zero");
	fh_clearall();
	pc_test();
	r = feraiseexcept(FE_ALL_EXCEPT);
	pc_check(r == 0, "feraiseexcept(FE_ALL_EXCEPT) returned %d, expected 0", r);
	for (i = 0; i < FH_NEXC; i++) {
		e = fh_exc[i];
		pc_check((fetestexcept(e) & e) != 0, "exception 0x%x not set after feraiseexcept(FE_ALL_EXCEPT)", e);
	}
	fh_clearall();
	pc_pass();
}
