/*
 * A handler run by raise() is entered with ESP + 4 a multiple of 16.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.signals.001
 */
#include "../../../../check.h"
#include "../../../../sigtest.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.signals.001")
#else

int main(void)
{
	pc_begin("ABI.i386.signals.001");
	abi_require_sse();
	sig_install();
	pc_test();
	raise(SIGUSR1);
	pc_check(sig_got == SIGUSR1, "the handler was not run");
	abi_expect_entry_mod16(&sig_tramp_rec, "the signal handler");
	pc_pass();
}
#endif
