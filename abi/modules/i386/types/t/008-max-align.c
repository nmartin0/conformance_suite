/*
 * _Alignof(max_align_t) is 16.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.types.008
 */
#include "../../../../check.h"
#include <stddef.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.types.008")
#else

int main(void)
{
	pc_begin("ABI.i386.types.008");
	pc_test();
	_Static_assert(_Alignof(max_align_t) == 16, "the alignment of max_align_t must be 16");
	pc_pass();
}
#endif
