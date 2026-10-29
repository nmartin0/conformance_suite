/*
 * A handler that interrupts code running with the direction flag set is entered with it clear.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.signals.003
 */
#include "../../../../check.h"
#include "../../../../sigtest.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.signals.003")
#else

int main(void)
{
	pc_begin("ABI.i386.signals.003");
	struct abi_spin s;

	abi_require_sse();
	sig_install();
	pc_test();
	sig_async(&s, 0, 1);
	abi_expect_entry_df_clear(&sig_tramp_rec, "the signal handler");
	pc_pass();
}
#endif
