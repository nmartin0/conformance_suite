/*
 * long long is 8 bytes but only 4-byte aligned, also as a structure member.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.types.002
 */
#include "../../../../check.h"
#include <stddef.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.types.002")
#else
struct ll1 { char c; long long x; };
struct ll2 { char c; unsigned long long x; };
struct ll3 { long long x; char c; };
int main(void)
{
	pc_begin("ABI.i386.types.002");
	pc_test();
	_Static_assert(sizeof(long long) == 8, "long long must be 8 bytes");
	_Static_assert(sizeof(unsigned long long) == 8, "unsigned long long must be 8 bytes");
	_Static_assert(_Alignof(long long) == 4, "long long must have alignment 4");
	_Static_assert(_Alignof(unsigned long long) == 4, "unsigned long long must have alignment 4");
	_Static_assert(offsetof(struct ll1, x) == 4, "a long long member after a char must be at offset 4");
	_Static_assert(sizeof(struct ll1) == 12 && _Alignof(struct ll1) == 4, "struct { char; long long } must be 12 bytes, alignment 4");
	_Static_assert(offsetof(struct ll2, x) == 4, "an unsigned long long member after a char must be at offset 4");
	_Static_assert(sizeof(struct ll3) == 12 && _Alignof(struct ll3) == 4, "struct { long long; char } must be 12 bytes, alignment 4");
	pc_pass();
}
#endif
