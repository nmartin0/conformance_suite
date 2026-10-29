/*
 * EBX, ESI, EDI and EBP are preserved across memcpy.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.002
 */
#include "../../../../check.h"
#include "../../../../libcalls.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.002")
#else

int main(void)
{
	pc_begin("ABI.i386.calling.002");
	pc_test();
	{
		struct abi_call c;

		abi_lib_run(1, &c, 0);
		abi_expect_callee_saved(&c, "memcpy");
	}
	pc_pass();
}
#endif
