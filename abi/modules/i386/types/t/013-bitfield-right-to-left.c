/*
 * Bit-fields are allocated from the least significant bit upward within their storage unit.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.types.013
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.types.013")
#else
struct b1 { int j:5; int k:6; int m:7; };
int main(void)
{
	pc_begin("ABI.i386.types.013");
	pc_test();
	_Static_assert(sizeof(struct b1) == 4 && _Alignof(struct b1) == 4, "three int bit-fields of 5, 6, 7 bits must make a 4-byte, 4-aligned structure");
	{
		union { struct b1 s; unsigned w; } u;

		u.w = 0;
		u.s.j = 1;
		pc_check(u.w == 1u, "j = 1 gives the word 0x%08x, expected 0x00000001", u.w);
		u.w = 0;
		u.s.k = 1;
		pc_check(u.w == (1u << 5), "k = 1 gives the word 0x%08x, expected bit 5", u.w);
		u.w = 0;
		u.s.m = 1;
		pc_check(u.w == (1u << 11), "m = 1 gives the word 0x%08x, expected bit 11", u.w);
		u.w = 0;
		u.s.j = -1;
		pc_check(u.w == 0x1Fu, "j = -1 gives the word 0x%08x, expected 0x1f", u.w);
	}
	pc_pass();
}
#endif
