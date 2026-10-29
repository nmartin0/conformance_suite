/*
 * After raise() returns (its handler having changed registers and control words), the caller's callee-
 * saved registers, DF and control words are as before.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.signals.004
 */
#include "../../../../check.h"
#include "../../../../sigtest.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.signals.004")
#else

int main(void)
{
	pc_begin("ABI.i386.signals.004");
	struct abi_call c;

	abi_require_sse();
	sig_install();
	pc_test();
	abi_call_init(&c, (void *)(size_t)&raise);
	abi_call_arg(&c, SIGUSR1);
	c.flags |= ABI_CALL_SETFP;
	c.cw_set = ABI_X87_CW_ALT;
	c.mxcsr_set = ABI_MXCSR_ALT;
	abi_call(&c);
	if (sig_got != SIGUSR1)
		pc_unresolved("raise() did not run the handler");
	if (c.cw_before != ABI_X87_CW_ALT)
		pc_unresolved("could not load the x87 control word 0x%04x (it reads 0x%04x)", ABI_X87_CW_ALT, c.cw_before);
	abi_expect_callee_saved(&c, "raise and its handler");
	abi_expect_df_clear(&c, "raise and its handler");
	abi_expect_x87_cw_kept(&c, "raise and its handler");
	abi_expect_mxcsr_kept(&c, "raise and its handler");
	pc_pass();
}
#endif
