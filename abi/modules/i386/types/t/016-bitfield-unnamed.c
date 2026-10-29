/*
 * Unnamed bit-fields take space but do not change a structure's alignment.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.types.016
 */
#include "../../../../check.h"
#include <stddef.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.types.016")
#else
struct b5 { char c; int :0; char d; short :9; char e; char :0; };
struct b6 { char c; int :3; };
int main(void)
{
	pc_begin("ABI.i386.types.016");
	pc_test();
	_Static_assert(sizeof(struct b5) == 9 && _Alignof(struct b5) == 1, "the unnamed-bit-field structure must be 9 bytes, alignment 1");
	_Static_assert(offsetof(struct b5, d) == 4, "the char after the zero-width int field must be at offset 4");
	_Static_assert(offsetof(struct b5, e) == 8, "the last named char must be at offset 8");
	_Static_assert(_Alignof(struct b6) == 1, "an unnamed int bit-field must not raise the alignment of the structure");
	pc_pass();
}
#endif
