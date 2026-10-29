/*
 * Nested aggregates keep their own alignment; a union has its most aligned member's alignment and its
 * size is rounded up to it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.types.012
 */
#include "../../../../check.h"
#include <stddef.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.types.012")
#else
struct in1 { char c; double d; };
struct in2 { char c[3]; };
struct out1 { char c; struct in1 x; char e; };
struct out2 { char c; struct in2 x; };
union u1 { char c; double d; };
union u2 { char c[5]; int i; };
union u3 { char c[5]; short s; };
union u4 { char c; long double d; };
union u5 { int i; char c[5]; };
struct out3 { char c; union u5 u; };
int main(void)
{
	pc_begin("ABI.i386.types.012");
	pc_test();
	_Static_assert(sizeof(struct in1) == 12 && _Alignof(struct in1) == 4, "struct { char; double } is 12 bytes, alignment 4");
	_Static_assert(offsetof(struct out1, x) == 4 && offsetof(struct out1, e) == 16 && sizeof(struct out1) == 20,
	    "char, struct{char;double}, char: offsets 0, 4, 16 and size 20");
	_Static_assert(offsetof(struct out2, x) == 1 && sizeof(struct out2) == 4 && _Alignof(struct out2) == 1,
	    "a struct of three chars after a char sits at offset 1: size 4, alignment 1");
	_Static_assert(sizeof(union u1) == 8 && _Alignof(union u1) == 4, "union { char; double } must be 8 bytes, alignment 4");
	_Static_assert(sizeof(union u2) == 8 && _Alignof(union u2) == 4, "union { char[5]; int } must be 8 bytes, alignment 4");
	_Static_assert(sizeof(union u3) == 6 && _Alignof(union u3) == 2, "union { char[5]; short } must be 6 bytes, alignment 2");
	_Static_assert(sizeof(union u4) == 12 && _Alignof(union u4) == 4, "union { char; long double; } must be 12 bytes, alignment 4");
	_Static_assert(offsetof(struct out3, u) == 4 && sizeof(struct out3) == 12, "char then union { int; char[5] }: offset 4, size 12");
	_Static_assert(sizeof(struct in1[3]) == 36, "an array of three struct { char; double } must be 36 bytes");
	pc_pass();
}
#endif
