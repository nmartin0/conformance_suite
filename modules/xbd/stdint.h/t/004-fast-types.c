/*
 * Fastest minimum-width integer types and their limits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdint.h.004
 */
#include <stdint.h>
#include <limits.h>
#include "pc.h"
#define TYID(e) _Generic((e), char: 1, signed char: 2, unsigned char: 3, short: 4, unsigned short: 5, \
	int: 6, unsigned int: 7, long: 8, unsigned long: 9, long long: 10, unsigned long long: 11, default: 0)

#if INT_FAST8_MAX < 127 || INT_FAST8_MIN > (-127 - 1) || UINT_FAST8_MAX < 255
#error "a limit of the 8-bit fast types is too small in magnitude"
#endif
_Static_assert(TYID(INT_FAST8_MAX) == TYID(+(int_fast8_t)0) && TYID(INT_FAST8_MIN) == TYID(+(int_fast8_t)0) && TYID(UINT_FAST8_MAX) == TYID(+(uint_fast8_t)0),
	"a limit of the 8-bit fast types does not have the promoted type");
_Static_assert((int_fast8_t)-1 < 0, "int_fast8_t is not signed");
_Static_assert((uint_fast8_t)-1 > 0, "uint_fast8_t is not unsigned");
_Static_assert(sizeof(int_fast8_t) * CHAR_BIT >= 8 && sizeof(uint_fast8_t) * CHAR_BIT >= 8, "a 8-bit fast type is narrower than 8 bits");
#if INT_FAST16_MAX < 32767 || INT_FAST16_MIN > (-32767 - 1) || UINT_FAST16_MAX < 65535
#error "a limit of the 16-bit fast types is too small in magnitude"
#endif
_Static_assert(TYID(INT_FAST16_MAX) == TYID(+(int_fast16_t)0) && TYID(INT_FAST16_MIN) == TYID(+(int_fast16_t)0) && TYID(UINT_FAST16_MAX) == TYID(+(uint_fast16_t)0),
	"a limit of the 16-bit fast types does not have the promoted type");
_Static_assert((int_fast16_t)-1 < 0, "int_fast16_t is not signed");
_Static_assert((uint_fast16_t)-1 > 0, "uint_fast16_t is not unsigned");
_Static_assert(sizeof(int_fast16_t) * CHAR_BIT >= 16 && sizeof(uint_fast16_t) * CHAR_BIT >= 16, "a 16-bit fast type is narrower than 16 bits");
#if INT_FAST32_MAX < 2147483647 || INT_FAST32_MIN > (-2147483647 - 1) || UINT_FAST32_MAX < 4294967295U
#error "a limit of the 32-bit fast types is too small in magnitude"
#endif
_Static_assert(TYID(INT_FAST32_MAX) == TYID(+(int_fast32_t)0) && TYID(INT_FAST32_MIN) == TYID(+(int_fast32_t)0) && TYID(UINT_FAST32_MAX) == TYID(+(uint_fast32_t)0),
	"a limit of the 32-bit fast types does not have the promoted type");
_Static_assert((int_fast32_t)-1 < 0, "int_fast32_t is not signed");
_Static_assert((uint_fast32_t)-1 > 0, "uint_fast32_t is not unsigned");
_Static_assert(sizeof(int_fast32_t) * CHAR_BIT >= 32 && sizeof(uint_fast32_t) * CHAR_BIT >= 32, "a 32-bit fast type is narrower than 32 bits");
#if INT_FAST64_MAX < 9223372036854775807LL || INT_FAST64_MIN > (-9223372036854775807LL - 1) || UINT_FAST64_MAX < 18446744073709551615ULL
#error "a limit of the 64-bit fast types is too small in magnitude"
#endif
_Static_assert(TYID(INT_FAST64_MAX) == TYID(+(int_fast64_t)0) && TYID(INT_FAST64_MIN) == TYID(+(int_fast64_t)0) && TYID(UINT_FAST64_MAX) == TYID(+(uint_fast64_t)0),
	"a limit of the 64-bit fast types does not have the promoted type");
_Static_assert((int_fast64_t)-1 < 0, "int_fast64_t is not signed");
_Static_assert((uint_fast64_t)-1 > 0, "uint_fast64_t is not unsigned");
_Static_assert(sizeof(int_fast64_t) * CHAR_BIT >= 64 && sizeof(uint_fast64_t) * CHAR_BIT >= 64, "a 64-bit fast type is narrower than 64 bits");

int main(void)
{
	pc_begin("XBD.stdint.h.004");
	pc_test();
	pc_pass();
}
