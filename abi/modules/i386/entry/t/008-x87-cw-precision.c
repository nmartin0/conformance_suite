/*
 * At entry the x87 precision control selects the extended (64-bit) precision.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.entry.008
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.entry.008")
#else
int main(void)
{
	unsigned cw;

	cw = abi_x87_cw();      /* first, before the test helper runs */
	pc_begin("ABI.i386.entry.008");
	pc_test();
	/* Follows the current revision (field 11, extended). The older table gives 11 but calls it 53-bit. */
	pc_check((cw & ABI_X87_CW_PC_MASK) == ABI_X87_CW_PC_MASK,
	    "x87 control word 0x%04x: precision control is 0x%x, not 3 (extended)", cw, (cw & ABI_X87_CW_PC_MASK) >> 8);
	pc_pass();
}
#endif
