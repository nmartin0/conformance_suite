/*
 * float is 4 bytes aligned to 4; double is 8 bytes but only 4-byte aligned, also in structures.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.types.004
 */
#include "../../../../check.h"
#include <stddef.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.types.004")
#else
struct d1 { char c; double x; };
struct d2 { double x; char c; };
struct d3 { char c; float x; };
int main(void)
{
	pc_begin("ABI.i386.types.004");
	pc_test();
	_Static_assert(sizeof(float) == 4 && _Alignof(float) == 4, "float must be 4 bytes, alignment 4");
	_Static_assert(sizeof(double) == 8, "double must be 8 bytes");
	_Static_assert(_Alignof(double) == 4, "double must have alignment 4");
	_Static_assert(offsetof(struct d1, x) == 4, "a double member after a char must be at offset 4");
	_Static_assert(sizeof(struct d1) == 12 && _Alignof(struct d1) == 4, "struct { char; double } must be 12 bytes, alignment 4");
	_Static_assert(sizeof(struct d2) == 12 && _Alignof(struct d2) == 4, "struct { double; char } must be 12 bytes, alignment 4");
	_Static_assert(offsetof(struct d3, x) == 4 && sizeof(struct d3) == 8, "struct { char; float } must have the float at 4, size 8");
	_Static_assert(sizeof(double[3]) == 24, "an array of three doubles must be 24 bytes");
	pc_pass();
}
#endif
