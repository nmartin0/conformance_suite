/*
 * The x87 control word of interrupted code is restored after the handler, whether the signal is
 * asynchronous or not.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.signals.006
 */
#include "../../../../check.h"
#include "../../../../sigtest.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.signals.006")
#else

int main(void)
{
	pc_begin("ABI.i386.signals.006");
	struct abi_spin s;

	abi_require_sse();
	sig_install();
	pc_test();
	sig_async(&s, 0, 0);
	abi_expect_resumed_cw(&s, "after the signal handler");
	pc_pass();
}
#endif
