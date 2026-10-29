/*
 * long double is 12 bytes, 4-byte aligned, also in structures.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.types.005
 */
#include "../../../../check.h"
#include <stddef.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.types.005")
#else
struct l1 { char c; long double x; };
struct l2 { long double x; char c; };
int main(void)
{
	pc_begin("ABI.i386.types.005");
	pc_test();
	_Static_assert(sizeof(long double) == 12, "long double must be 12 bytes");
	_Static_assert(_Alignof(long double) == 4, "long double must have alignment 4");
	_Static_assert(offsetof(struct l1, x) == 4, "a long double member after a char must be at offset 4");
	_Static_assert(sizeof(struct l1) == 16 && _Alignof(struct l1) == 4, "struct { char; long double } must be 16 bytes, alignment 4");
	_Static_assert(sizeof(struct l2) == 16 && _Alignof(struct l2) == 4, "struct { long double; char } must be 16 bytes, alignment 4");
	_Static_assert(sizeof(long double[2]) == 24, "an array of two long doubles must be 24 bytes");
	pc_pass();
}
#endif
