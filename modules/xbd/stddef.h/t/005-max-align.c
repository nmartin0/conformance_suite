/*
 * max_align_t is aligned at least as strictly as every scalar type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stddef.h.005
 */
#include <stddef.h>
#include "pc.h"

#define A _Alignof(max_align_t)
_Static_assert(A >= _Alignof(char) && A >= _Alignof(short) && A >= _Alignof(int) && A >= _Alignof(long) && A >= _Alignof(long long), "max_align_t is aligned less than an integer type");
_Static_assert(A >= _Alignof(float) && A >= _Alignof(double) && A >= _Alignof(long double), "max_align_t is aligned less than a floating type");
_Static_assert(A >= _Alignof(void *) && A >= _Alignof(void (*)(void)) && A >= _Alignof(wchar_t) && A >= _Alignof(size_t) && A >= _Alignof(ptrdiff_t), "max_align_t is aligned less than a pointer or size type");
_Static_assert((A & (A - 1)) == 0, "the alignment of max_align_t is not a power of two");
_Static_assert(sizeof(max_align_t) % A == 0, "the size of max_align_t is not a multiple of its alignment");

int main(void)
{
	pc_begin("XBD.stddef.h.005");
	pc_test();
	pc_pass();
}
