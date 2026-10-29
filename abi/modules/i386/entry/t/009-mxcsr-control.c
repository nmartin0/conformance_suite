/*
 * At entry the MXCSR control bits are the defaults: all exceptions masked, round to nearest, no FZ, no DAZ.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.entry.009
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.entry.009")
#else
int main(void)
{
	unsigned m = 0;
	int sse = abi_cpu_has_sse();

	if (sse)
		m = abi_mxcsr();        /* first, before the test helper runs */
	pc_begin("ABI.i386.entry.009");
	if (!sse)
		abi_require_sse();
	pc_test();
	pc_check((m & ABI_MXCSR_CONTROL) == (ABI_MXCSR_DEFAULT & ABI_MXCSR_CONTROL),
	    "MXCSR is 0x%08x; its control bits are 0x%04x, expected 0x%04x", m,
	    m & ABI_MXCSR_CONTROL, ABI_MXCSR_DEFAULT & ABI_MXCSR_CONTROL);
	pc_pass();
}
#endif
