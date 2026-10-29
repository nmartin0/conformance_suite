/*
 * size_t is unsigned int.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.types.009
 */
#include "../../../../check.h"
#include <stddef.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.types.009")
#else

int main(void)
{
	pc_begin("ABI.i386.types.009");
	pc_test();
	_Static_assert(_Generic((size_t)0, unsigned int: 1, default: 0), "size_t must be unsigned int");
	_Static_assert(_Generic(sizeof(int), unsigned int: 1, default: 0), "the type of sizeof must be unsigned int");
	pc_pass();
}
#endif
