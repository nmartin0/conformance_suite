/*
 * feclearexcept(FE_ALL_EXCEPT) clears every exception.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.feclearexcept.005
 */
#include "fh.h"

int main(void)
{
	int i, r;

	pc_begin("XSH.feclearexcept.005");
	if (FE_ALL_EXCEPT == 0)
		pc_notinuse("FE_ALL_EXCEPT is zero");
	pc_test();
	for (i = 0; i < FH_NEXC; i++)
		fh_raise(fh_exc[i]);
	r = feclearexcept(FE_ALL_EXCEPT);
	pc_check(r == 0, "feclearexcept(FE_ALL_EXCEPT) returned %d, expected 0", r);
	r = fetestexcept(FE_ALL_EXCEPT);
	pc_check(r == 0, "flags 0x%x still set after feclearexcept(FE_ALL_EXCEPT)", r);
	pc_pass();
}
