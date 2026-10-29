/*
 * int, long, enumeration and pointer values come back in EAX.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.022
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.022")
#else
enum color { RED = 1234 };
static int marker;
__attribute__((noinline)) int f_int(void) { return 0x7EDCBA98; }
__attribute__((noinline)) long f_long(void) { return (long)0x80000001u; }
__attribute__((noinline)) unsigned f_uint(void) { return 0xFFFFFFFEu; }
__attribute__((noinline)) enum color f_enum(void) { return RED; }
__attribute__((noinline)) int *f_ptr(void) { return &marker; }
__attribute__((noinline)) int (*f_fptr(void))(void) { return f_int; }
int main(void)
{
	pc_begin("ABI.i386.calling.022");
	struct abi_call c;

	pc_test();
	abi_call_init(&c, (void *)(size_t)&f_int);
	abi_call(&c);
	pc_check(c.eax == 0x7EDCBA98u, "int returned 0x%08x in EAX", c.eax);
	abi_call_init(&c, (void *)(size_t)&f_long);
	abi_call(&c);
	pc_check(c.eax == 0x80000001u, "long returned 0x%08x in EAX", c.eax);
	abi_call_init(&c, (void *)(size_t)&f_uint);
	abi_call(&c);
	pc_check(c.eax == 0xFFFFFFFEu, "unsigned int returned 0x%08x in EAX", c.eax);
	abi_call_init(&c, (void *)(size_t)&f_enum);
	abi_call(&c);
	pc_check(c.eax == 1234, "enum returned 0x%08x in EAX", c.eax);
	abi_call_init(&c, (void *)(size_t)&f_ptr);
	abi_call(&c);
	pc_check(c.eax == (unsigned)(size_t)&marker, "pointer returned 0x%08x in EAX, expected 0x%08x", c.eax, (unsigned)(size_t)&marker);
	abi_call_init(&c, (void *)(size_t)&f_fptr);
	abi_call(&c);
	pc_check(c.eax == (unsigned)(size_t)&f_int, "function pointer returned 0x%08x in EAX", c.eax);
	pc_pass();
}
#endif
