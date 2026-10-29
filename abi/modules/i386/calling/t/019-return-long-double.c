/*
 * long double comes back in st0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.019
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.019")
#else
__attribute__((noinline)) long double f_ldouble(void) { return 3.14159265358979323846L; }
int main(void)
{
	pc_begin("ABI.i386.calling.019");
	struct abi_call c;

	pc_test();
	abi_call_init(&c, (void *)(size_t)&f_ldouble);
	abi_call(&c);
	abi_require_x87_empty(&c);
	abi_expect_st0(&c, 3.14159265358979323846L, "f_ldouble");
	pc_pass();
}
#endif
