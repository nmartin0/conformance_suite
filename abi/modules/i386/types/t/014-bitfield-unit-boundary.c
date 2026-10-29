/*
 * A bit-field lies wholly inside a storage unit of its declared type and shares it with other members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.types.014
 */
#include "../../../../check.h"
#include <stddef.h>
#include <string.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.types.014")
#else
struct b2 { short s:9; int j:9; char c; short t:9; short u:9; char d; };
int main(void)
{
	pc_begin("ABI.i386.types.014");
	pc_test();
	_Static_assert(sizeof(struct b2) == 12 && _Alignof(struct b2) == 4, "the mixed bit-field structure must be 12 bytes, alignment 4");
	_Static_assert(offsetof(struct b2, c) == 3, "the char after the 9-bit int field must be at offset 3");
	_Static_assert(offsetof(struct b2, d) == 8, "the last char must be at offset 8");
	{
		union { struct b2 s; unsigned w[3]; } u;

		memset(&u, 0, sizeof u);
		u.s.s = 1;
		u.s.j = 1;
		u.s.c = 0x55;
		u.s.t = 1;
		u.s.u = 1;
		u.s.d = 0x66;
		pc_check(u.w[0] == (1u | (1u << 9) | (0x55u << 24)), "first word is 0x%08x, expected 0x55000201", u.w[0]);
		pc_check(u.w[1] == (1u | (1u << 16)), "second word is 0x%08x, expected 0x00010001", u.w[1]);
		pc_check((u.w[2] & 0xFFu) == 0x66u, "third word is 0x%08x, expected the char 0x66 in its low byte", u.w[2]);
	}
	pc_pass();
}
#endif
