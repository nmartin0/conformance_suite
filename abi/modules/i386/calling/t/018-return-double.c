/*
 * double comes back in st0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.018
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.018")
#else
__attribute__((noinline)) double f_double(void) { return -2.75; }
int main(void)
{
	pc_begin("ABI.i386.calling.018");
	struct abi_call c;

	pc_test();
	abi_call_init(&c, (void *)(size_t)&f_double);
	abi_call(&c);
	abi_require_x87_empty(&c);
	abi_expect_st0(&c, -2.75L, "f_double");
	pc_pass();
}
#endif
