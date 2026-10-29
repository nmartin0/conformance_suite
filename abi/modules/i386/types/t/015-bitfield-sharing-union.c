/*
 * A bit-field shares a unit with an earlier member; in a union it starts at the unit's low bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.types.015
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.types.015")
#else
struct b3 { char c; short s:8; };
union b4 { char c; short s:8; };
int main(void)
{
	pc_begin("ABI.i386.types.015");
	pc_test();
	_Static_assert(sizeof(struct b3) == 2 && _Alignof(struct b3) == 2, "struct { char; short:8 } must be 2 bytes, alignment 2");
	_Static_assert(sizeof(union b4) == 2 && _Alignof(union b4) == 2, "union { char; short:8 } must be 2 bytes, alignment 2");
	{
		union { struct b3 s; unsigned short w; } a;
		union { union b4 v; unsigned short w; } b;

		a.w = 0;
		a.s.c = 0x11;
		a.s.s = 0x22;
		pc_check(a.w == 0x2211, "struct { char c; short s:8 } holds 0x%04x for c = 0x11, s = 0x22; expected 0x2211", a.w);
		b.w = 0;
		b.v.s = 0x22;
		pc_check(b.w == 0x0022, "union { char c; short s:8 } holds 0x%04x for s = 0x22; expected 0x0022", b.w);
	}
	pc_pass();
}
#endif
