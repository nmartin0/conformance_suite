/*
 * A handler that interrupts running code is entered with ESP + 4 a multiple of 16, whatever the
 * interrupted ESP.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.signals.002
 */
#include "../../../../check.h"
#include "../../../../sigtest.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.signals.002")
#else

int main(void)
{
	pc_begin("ABI.i386.signals.002");
	struct abi_spin s;
	unsigned m;

	abi_require_sse();
	sig_install();
	pc_test();
	for (m = 0; m < 16; m += 4) {
		memset((void *)&sig_tramp_rec, 0, sizeof sig_tramp_rec);
		sig_async(&s, m, 0);
		abi_expect_entry_mod16(&sig_tramp_rec, "the signal handler");
	}
	pc_pass();
}
#endif
