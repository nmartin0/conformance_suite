/*
 * Exact values and types of the exact-width limit macros.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdint.h.002
 */
#include <stdint.h>
#include <limits.h>
#include "pc.h"
#define TYID(e) _Generic((e), char: 1, signed char: 2, unsigned char: 3, short: 4, unsigned short: 5, \
	int: 6, unsigned int: 7, long: 8, unsigned long: 9, long long: 10, unsigned long long: 11, default: 0)

#if INT8_MIN != (-127 - 1) || INT8_MAX != 127 || UINT8_MAX != 255
#error "an exact-width limit for 8 bits is not the required value"
#endif
_Static_assert(TYID(INT8_MIN) == TYID(+(int8_t)0) && TYID(INT8_MAX) == TYID(+(int8_t)0) && TYID(UINT8_MAX) == TYID(+(uint8_t)0), "a 8-bit exact-width limit does not have the promoted type");
#if INT16_MIN != (-32767 - 1) || INT16_MAX != 32767 || UINT16_MAX != 65535
#error "an exact-width limit for 16 bits is not the required value"
#endif
_Static_assert(TYID(INT16_MIN) == TYID(+(int16_t)0) && TYID(INT16_MAX) == TYID(+(int16_t)0) && TYID(UINT16_MAX) == TYID(+(uint16_t)0), "a 16-bit exact-width limit does not have the promoted type");
#if INT32_MIN != (-2147483647 - 1) || INT32_MAX != 2147483647 || UINT32_MAX != 4294967295U
#error "an exact-width limit for 32 bits is not the required value"
#endif
_Static_assert(TYID(INT32_MIN) == TYID(+(int32_t)0) && TYID(INT32_MAX) == TYID(+(int32_t)0) && TYID(UINT32_MAX) == TYID(+(uint32_t)0), "a 32-bit exact-width limit does not have the promoted type");

#ifdef INT64_MAX
#if INT64_MIN != (-9223372036854775807LL - 1) || INT64_MAX != 9223372036854775807LL || UINT64_MAX != 18446744073709551615ULL
#error "an exact-width limit for 64 bits is not the required value"
#endif
_Static_assert(TYID(INT64_MIN) == TYID(+(int64_t)0) && TYID(INT64_MAX) == TYID(+(int64_t)0) && TYID(UINT64_MAX) == TYID(+(uint64_t)0),
	"a 64-bit exact-width limit does not have the promoted type");
#endif

int main(void)
{
	pc_begin("XBD.stdint.h.002");
	pc_test();
	pc_pass();
}
