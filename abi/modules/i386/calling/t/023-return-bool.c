/*
 * A _Bool return value is 0 or 1 in AL, bits 1 to 7 zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.023
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.023")
#else
__attribute__((noinline)) _Bool f_true(void) { return 1; }
__attribute__((noinline)) _Bool f_false(void) { return 0; }
__attribute__((noinline)) _Bool f_cmp(int x) { return x > 3; }
int main(void)
{
	pc_begin("ABI.i386.calling.023");
	struct abi_call c;

	pc_test();
	abi_call_init(&c, (void *)(size_t)&f_true);
	abi_call(&c);
	pc_check((c.eax & 0xFFu) == 1, "a function returning true left AL = 0x%02x", c.eax & 0xFFu);
	abi_call_init(&c, (void *)(size_t)&f_false);
	abi_call(&c);
	pc_check((c.eax & 0xFFu) == 0, "a function returning false left AL = 0x%02x", c.eax & 0xFFu);
	abi_call_init(&c, (void *)(size_t)&f_cmp);
	abi_call_arg(&c, 77);
	abi_call(&c);
	pc_check((c.eax & 0xFFu) == 1, "a function returning (77 > 3) left AL = 0x%02x", c.eax & 0xFFu);
	pc_pass();
}
#endif
