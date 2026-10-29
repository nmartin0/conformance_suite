/*
 * The x87 control word survives strlen, memcpy, qsort and snprintf.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.006
 */
#include "../../../../check.h"
#include "../../../../libcalls.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.006")
#else

int main(void)
{
	pc_begin("ABI.i386.calling.006");
	pc_test();
	{
		struct abi_call c;
		int k;

		for (k = 0; k < ABI_LIB_N; k++) {
			abi_lib_run(k, &c, 1);
			abi_expect_x87_cw_kept(&c, abi_lib_name[k]);
		}
	}
	pc_pass();
}
#endif
