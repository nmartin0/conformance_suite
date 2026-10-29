/*
 * A structure's size is a multiple of its alignment, which is that of its most aligned member.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.types.011
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.types.011")
#else
struct t1 { int i; char c; };
struct t2 { short s; char c; };
struct t3 { char c[5]; };
struct t4 { int i; char c; short s; char d; };
struct t5 { short s[3]; };
struct t6 { char c; };
int main(void)
{
	pc_begin("ABI.i386.types.011");
	pc_test();
	_Static_assert(sizeof(struct t1) == 8 && _Alignof(struct t1) == 4, "struct { int; char } must be 8 bytes, alignment 4");
	_Static_assert(sizeof(struct t2) == 4 && _Alignof(struct t2) == 2, "struct { short; char } must be 4 bytes, alignment 2");
	_Static_assert(sizeof(struct t3) == 5 && _Alignof(struct t3) == 1, "struct { char[5] } must be 5 bytes, alignment 1");
	_Static_assert(sizeof(struct t4) == 12 && _Alignof(struct t4) == 4, "struct { int; char; short; char } must be 12 bytes, alignment 4");
	_Static_assert(sizeof(struct t5) == 6 && _Alignof(struct t5) == 2, "struct { short[3] } must be 6 bytes, alignment 2");
	_Static_assert(sizeof(struct t6) == 1 && _Alignof(struct t6) == 1, "struct { char } must be 1 byte, alignment 1");
	_Static_assert(sizeof(struct t1[3]) == 24, "an array of three struct { int; char } must be 24 bytes");
	pc_pass();
}
#endif
