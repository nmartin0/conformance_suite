/*
 * _Bool is one byte and, in memory, always holds 0 or 1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.types.007
 */
#include "../../../../check.h"
#include <string.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.types.007")
#else

int main(void)
{
	pc_begin("ABI.i386.types.007");
	pc_test();
	_Static_assert(sizeof(_Bool) == 1 && _Alignof(_Bool) == 1, "_Bool must be 1 byte, alignment 1");
	{
		volatile int nonzero = 256;
		volatile double half = 0.5;
		_Bool b = nonzero, c = half, z = 0;
		unsigned char raw;

		memcpy(&raw, &b, 1);
		pc_check(raw == 1, "_Bool set from a non-zero int is stored as 0x%02x, not 1", raw);
		memcpy(&raw, &c, 1);
		pc_check(raw == 1, "_Bool set from a non-zero double is stored as 0x%02x, not 1", raw);
		memcpy(&raw, &z, 1);
		pc_check(raw == 0, "_Bool set to false is stored as 0x%02x, not 0", raw);
	}
	pc_pass();
}
#endif
