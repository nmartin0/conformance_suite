/*
 * At entry the x87 rounding control is "nearest" and all six exceptions are masked.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.entry.007
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.entry.007")
#else
int main(void)
{
	unsigned cw;

	cw = abi_x87_cw();      /* first, before the test helper runs */
	pc_begin("ABI.i386.entry.007");
	pc_test();
	pc_check((cw & ABI_X87_CW_RC_MASK) == 0, "x87 control word 0x%04x: rounding control is not 'nearest'", cw);
	pc_check((cw & ABI_X87_CW_MASKS) == ABI_X87_CW_MASKS,
	    "x87 control word 0x%04x: not all six exceptions are masked", cw);
	pc_pass();
}
#endif
