/*
 * char, short, int, long, enum: size and alignment, and char is signed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.types.001
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.types.001")
#else
enum e1 { E1A, E1B = 1000 };
enum e2 { E2A = -1, E2B = 2147483647 };
int main(void)
{
	pc_begin("ABI.i386.types.001");
	pc_test();
	_Static_assert(sizeof(char) == 1 && _Alignof(char) == 1, "char must be 1 byte, alignment 1");
	_Static_assert(sizeof(signed char) == 1 && _Alignof(signed char) == 1, "signed char must be 1 byte, alignment 1");
	_Static_assert(sizeof(unsigned char) == 1 && _Alignof(unsigned char) == 1, "unsigned char must be 1 byte, alignment 1");
	_Static_assert((char)-1 < 0, "plain char must be a signed byte");
	_Static_assert(sizeof(short) == 2 && _Alignof(short) == 2, "short must be 2 bytes, alignment 2");
	_Static_assert(sizeof(unsigned short) == 2 && _Alignof(unsigned short) == 2, "unsigned short must be 2 bytes, alignment 2");
	_Static_assert(sizeof(int) == 4 && _Alignof(int) == 4, "int must be 4 bytes, alignment 4");
	_Static_assert(sizeof(unsigned int) == 4 && _Alignof(unsigned int) == 4, "unsigned int must be 4 bytes, alignment 4");
	_Static_assert(sizeof(long) == 4 && _Alignof(long) == 4, "long must be 4 bytes, alignment 4");
	_Static_assert(sizeof(unsigned long) == 4 && _Alignof(unsigned long) == 4, "unsigned long must be 4 bytes, alignment 4");
	_Static_assert(sizeof(enum e1) == 4 && _Alignof(enum e1) == 4, "an enumeration type must be 4 bytes, alignment 4");
	_Static_assert(sizeof(enum e2) == 4 && _Alignof(enum e2) == 4, "an enumeration type with a negative constant must be 4 bytes, alignment 4");
	pc_pass();
}
#endif
