/*
 * Code interrupted while holding sentinels in EBX, ESI, EDI and EBP finds them again after the
 * handler.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.signals.005
 */
#include "../../../../check.h"
#include "../../../../sigtest.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.signals.005")
#else

int main(void)
{
	pc_begin("ABI.i386.signals.005");
	struct abi_spin s;
	unsigned m;

	abi_require_sse();
	sig_install();
	pc_test();
	for (m = 0; m < 16; m += 4) {
		sig_async(&s, m, 1);
		abi_expect_resumed_regs(&s, "after the signal handler");
		abi_expect_resumed_df(&s, 1, "after the signal handler");
	}
	pc_pass();
}
#endif
