/*
 * Complex types are twice their real type in size and have its alignment.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.types.006
 */
#include "../../../../check.h"
#include <stddef.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.types.006")
#else
struct c1 { char c; _Complex double x; };
int main(void)
{
	pc_begin("ABI.i386.types.006");
	pc_test();
	_Static_assert(sizeof(_Complex float) == 8 && _Alignof(_Complex float) == 4, "_Complex float must be 8 bytes, alignment 4");
	_Static_assert(sizeof(_Complex double) == 16 && _Alignof(_Complex double) == 4, "_Complex double must be 16 bytes, alignment 4");
	_Static_assert(sizeof(_Complex long double) == 24 && _Alignof(_Complex long double) == 4, "_Complex long double must be 24 bytes, alignment 4");
	_Static_assert(offsetof(struct c1, x) == 4, "a _Complex double member after a char must be at offset 4");
	pc_pass();
}
#endif
