/*
 * float comes back in st0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.017
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.017")
#else
__attribute__((noinline)) float f_float(void) { return 1.5f; }
int main(void)
{
	pc_begin("ABI.i386.calling.017");
	struct abi_call c;

	pc_test();
	abi_call_init(&c, (void *)(size_t)&f_float);
	abi_call(&c);
	abi_require_x87_empty(&c);
	abi_expect_st0(&c, 1.5L, "f_float");
	pc_pass();
}
#endif
