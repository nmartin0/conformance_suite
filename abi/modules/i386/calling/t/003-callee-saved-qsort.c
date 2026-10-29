/*
 * EBX, ESI, EDI and EBP are preserved across qsort and its callback.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.003
 */
#include "../../../../check.h"
#include "../../../../libcalls.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.003")
#else

int main(void)
{
	pc_begin("ABI.i386.calling.003");
	pc_test();
	{
		struct abi_call c;

		abi_lib_run(2, &c, 0);
		abi_expect_callee_saved(&c, "qsort");
	}
	pc_pass();
}
#endif
