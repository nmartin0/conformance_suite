/*
 * At entry no floating-point exception flag is set, in the x87 unit or in MXCSR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.entry.010
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.entry.010")
#else
int main(void)
{
	unsigned sw, m = 0;
	int sse = abi_cpu_has_sse();

	sw = abi_x87_sw();      /* first, before the test helper runs */
	if (sse)
		m = abi_mxcsr();
	pc_begin("ABI.i386.entry.010");
	if (!sse)
		abi_require_sse();
	pc_test();
	/* Observed in main(), after the start-up code of the C library has run. */
	pc_check((sw & 0x3Fu) == 0, "x87 status word 0x%04x has exception flags set", sw);
	pc_check((m & 0x3Fu) == 0, "MXCSR 0x%08x has exception flags set", m);
	pc_pass();
}
#endif
