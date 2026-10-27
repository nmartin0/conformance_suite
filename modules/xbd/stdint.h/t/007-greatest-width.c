/*
 * Greatest-width integer types and limits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdint.h.007
 */
#include <stdint.h>
#include <limits.h>
#include "pc.h"
#define TYID(e) _Generic((e), char: 1, signed char: 2, unsigned char: 3, short: 4, unsigned short: 5, \
	int: 6, unsigned int: 7, long: 8, unsigned long: 9, long long: 10, unsigned long long: 11, default: 0)

#if INTMAX_MAX < 9223372036854775807LL || INTMAX_MIN > (-9223372036854775807LL - 1) || UINTMAX_MAX < 18446744073709551615ULL
#error "an intmax_t limit is below the required magnitude"
#endif
#if INTMAX_MAX < LLONG_MAX || INTMAX_MIN > LLONG_MIN || UINTMAX_MAX < ULLONG_MAX
#error "intmax_t or uintmax_t cannot hold every long long or unsigned long long value"
#endif
#if INTMAX_MIN != (-INTMAX_MAX - 1)
#error "intmax_t is not two's complement"
#endif
_Static_assert((intmax_t)-1 < 0, "intmax_t is not signed");
_Static_assert((uintmax_t)-1 > 0, "uintmax_t is not unsigned");
_Static_assert(TYID(INTMAX_MIN) == TYID(+(intmax_t)0) && TYID(INTMAX_MAX) == TYID(+(intmax_t)0) && TYID(UINTMAX_MAX) == TYID(+(uintmax_t)0),
	"a greatest-width limit does not have the promoted type");

int main(void)
{
	pc_begin("XBD.stdint.h.007");
	pc_test();
	{
		intmax_t a = INTMAX_MIN, b = INTMAX_MAX;
		uintmax_t c = UINTMAX_MAX;
		long long ll = LLONG_MIN, lh = LLONG_MAX;
		unsigned long long ul = ULLONG_MAX;

		pc_check((long long)(intmax_t)ll == ll && (long long)(intmax_t)lh == lh, "intmax_t does not hold the long long range");
		pc_check((unsigned long long)(uintmax_t)ul == ul, "uintmax_t does not hold the unsigned long long range");
		pc_check(a < 0 && b > 0 && c == (uintmax_t)-1, "intmax_t or uintmax_t limits are inconsistent with the types");
	}
	pc_pass();
}
