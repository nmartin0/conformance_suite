/*
 * The MXCSR control bits of interrupted code are restored after the handler.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.signals.007
 */
#include "../../../../check.h"
#include "../../../../sigtest.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.signals.007")
#else

int main(void)
{
	pc_begin("ABI.i386.signals.007");
	struct abi_spin s;

	abi_require_sse();
	sig_install();
	pc_test();
	sig_async(&s, 0, 0);
	abi_expect_resumed_mxcsr(&s, "after the signal handler");
	pc_pass();
}
#endif
