/*
 * Types of the numerical limit macros.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.009
 */
#include <sys/types.h>
#include <limits.h>
#include "pc.h"
#define TYID(e) _Generic((e), char: 1, signed char: 2, unsigned char: 3, short: 4, unsigned short: 5, \
	int: 6, unsigned int: 7, long: 8, unsigned long: 9, long long: 10, unsigned long long: 11, default: 0)

_Static_assert(TYID(CHAR_MAX) == TYID(+(char)0), "CHAR_MAX does not have the promoted type of char");
_Static_assert(TYID(CHAR_MIN) == TYID(+(char)0), "CHAR_MIN does not have the promoted type of char");
_Static_assert(TYID(SCHAR_MAX) == TYID(+(signed char)0), "SCHAR_MAX does not have the promoted type of signed char");
_Static_assert(TYID(SCHAR_MIN) == TYID(+(signed char)0), "SCHAR_MIN does not have the promoted type of signed char");
_Static_assert(TYID(UCHAR_MAX) == TYID(+(unsigned char)0), "UCHAR_MAX does not have the promoted type of unsigned char");
_Static_assert(TYID(SHRT_MAX) == TYID(+(short)0), "SHRT_MAX does not have the promoted type of short");
_Static_assert(TYID(SHRT_MIN) == TYID(+(short)0), "SHRT_MIN does not have the promoted type of short");
_Static_assert(TYID(USHRT_MAX) == TYID(+(unsigned short)0), "USHRT_MAX does not have the promoted type of unsigned short");
_Static_assert(TYID(INT_MAX) == TYID(+(int)0), "INT_MAX does not have the promoted type of int");
_Static_assert(TYID(INT_MIN) == TYID(+(int)0), "INT_MIN does not have the promoted type of int");
_Static_assert(TYID(UINT_MAX) == TYID(+(unsigned int)0), "UINT_MAX does not have the promoted type of unsigned int");
_Static_assert(TYID(LONG_MAX) == TYID(+(long)0), "LONG_MAX does not have the promoted type of long");
_Static_assert(TYID(LONG_MIN) == TYID(+(long)0), "LONG_MIN does not have the promoted type of long");
_Static_assert(TYID(ULONG_MAX) == TYID(+(unsigned long)0), "ULONG_MAX does not have the promoted type of unsigned long");
_Static_assert(TYID(LLONG_MAX) == TYID(+(long long)0), "LLONG_MAX does not have the promoted type of long long");
_Static_assert(TYID(LLONG_MIN) == TYID(+(long long)0), "LLONG_MIN does not have the promoted type of long long");
_Static_assert(TYID(ULLONG_MAX) == TYID(+(unsigned long long)0), "ULLONG_MAX does not have the promoted type of unsigned long long");
_Static_assert(TYID(SSIZE_MAX) == TYID(+(ssize_t)0), "SSIZE_MAX does not have the promoted type of ssize_t");

int main(void)
{
	pc_begin("XBD.limits.h.009");
	pc_test();
	pc_pass();
}
