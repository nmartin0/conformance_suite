/*
 * long long and unsigned long long come back in EDX:EAX.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.021
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.021")
#else
__attribute__((noinline)) unsigned long long f_ull(void) { return 0x0123456789ABCDEFULL; }
__attribute__((noinline)) long long f_ll(void) { return -2LL; }
__attribute__((noinline)) long long f_ll2(void) { return 0x80000000LL; }
int main(void)
{
	pc_begin("ABI.i386.calling.021");
	struct abi_call c;

	pc_test();
	abi_call_init(&c, (void *)(size_t)&f_ull);
	abi_call(&c);
	abi_expect_edx_eax(&c, 0x0123456789ABCDEFULL, "f_ull");
	abi_call_init(&c, (void *)(size_t)&f_ll);
	abi_call(&c);
	abi_expect_edx_eax(&c, (unsigned long long)-2LL, "f_ll");
	abi_call_init(&c, (void *)(size_t)&f_ll2);
	abi_call(&c);
	abi_expect_edx_eax(&c, 0x80000000ULL, "f_ll2");
	pc_pass();
}
#endif
