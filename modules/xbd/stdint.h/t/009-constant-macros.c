/*
 * The integer constant macros.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdint.h.009
 */
#include <stdint.h>
#include <limits.h>
#include "pc.h"
#define TYID(e) _Generic((e), char: 1, signed char: 2, unsigned char: 3, short: 4, unsigned short: 5, \
	int: 6, unsigned int: 7, long: 8, unsigned long: 9, long long: 10, unsigned long long: 11, default: 0)

#if INT8_C(5) != 5 || INT8_C(127) != 127 || UINT8_C(255) != 255
#error "INT8_C or UINT8_C gives the wrong value"
#endif
#if INT16_C(5) != 5 || INT16_C(32767) != 32767 || UINT16_C(65535) != 65535
#error "INT16_C or UINT16_C gives the wrong value"
#endif
#if INT32_C(5) != 5 || INT32_C(2147483647) != 2147483647 || UINT32_C(4294967295) != 4294967295U || UINT32_C(0x1234) != 0x1234
#error "INT32_C or UINT32_C gives the wrong value"
#endif
#if INT64_C(5) != 5 || INT64_C(9223372036854775807) != 9223372036854775807LL || UINT64_C(18446744073709551615) != 18446744073709551615ULL || UINT64_C(0x123) != 0x123
#error "INT64_C or UINT64_C gives the wrong value"
#endif
#if INTMAX_C(5) != 5 || INTMAX_C(9223372036854775807) != 9223372036854775807LL || UINTMAX_C(18446744073709551615) != 18446744073709551615ULL
#error "INTMAX_C or UINTMAX_C gives the wrong value"
#endif
_Static_assert(TYID(INT8_C(5)) == TYID(+(int_least8_t)0) && TYID(UINT8_C(5)) == TYID(+(uint_least8_t)0), "INT8_C/UINT8_C type");
_Static_assert(TYID(INT16_C(5)) == TYID(+(int_least16_t)0) && TYID(UINT16_C(5)) == TYID(+(uint_least16_t)0), "INT16_C/UINT16_C type");
_Static_assert(TYID(INT32_C(5)) == TYID(+(int_least32_t)0) && TYID(UINT32_C(5)) == TYID(+(uint_least32_t)0), "INT32_C/UINT32_C type");
_Static_assert(TYID(INT64_C(5)) == TYID(+(int_least64_t)0) && TYID(UINT64_C(5)) == TYID(+(uint_least64_t)0), "INT64_C/UINT64_C type");
_Static_assert(_Generic(INTMAX_C(5), intmax_t: 1, default: 0), "INTMAX_C does not have type intmax_t");
_Static_assert(_Generic(UINTMAX_C(5), uintmax_t: 1, default: 0), "UINTMAX_C does not have type uintmax_t");

static const int_least8_t a8 = INT8_C(100);
static const uint_least64_t a64 = UINT64_C(12345678901);
static const intmax_t am = INTMAX_C(5);

int main(void)
{
	pc_begin("XBD.stdint.h.009");
	pc_test();
	pc_check(a8 == 100 && a64 == 12345678901ULL && am == 5, "constants made with INTN_C do not initialise objects correctly");
	pc_pass();
}
