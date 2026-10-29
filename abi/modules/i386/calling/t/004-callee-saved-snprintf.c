/*
 * EBX, ESI, EDI and EBP are preserved across snprintf with floating-point arguments.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.004
 */
#include "../../../../check.h"
#include "../../../../libcalls.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.004")
#else

int main(void)
{
	pc_begin("ABI.i386.calling.004");
	pc_test();
	{
		struct abi_call c;

		abi_lib_run(3, &c, 0);
		abi_expect_callee_saved(&c, "snprintf");
	}
	pc_pass();
}
#endif
