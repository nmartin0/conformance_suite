/*
 * Minimum acceptable values of the integer limits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.007
 */
#include <limits.h>
#include "pc.h"

#if INT_MAX < 2147483647
#error "INT_MAX is below 2147483647"
#endif
#if INT_MIN > (-2147483647 - 1)
#error "INT_MIN is above -2147483648"
#endif
#if UINT_MAX < 4294967295
#error "UINT_MAX is below 4294967295"
#endif
#if SHRT_MAX < 32767
#error "SHRT_MAX is below 32767"
#endif
#if SHRT_MIN > (-32767 - 1)
#error "SHRT_MIN is above -32768"
#endif
#if USHRT_MAX < 65535
#error "USHRT_MAX is below 65535"
#endif
#if LONG_MAX < 2147483647L
#error "LONG_MAX is below 2147483647"
#endif
#if LONG_MIN > (-2147483647L - 1)
#error "LONG_MIN is above -2147483648"
#endif
#if ULONG_MAX < 4294967295UL
#error "ULONG_MAX is below 4294967295"
#endif
#if LLONG_MAX < 9223372036854775807LL
#error "LLONG_MAX is below 9223372036854775807"
#endif
#if LLONG_MIN > (-9223372036854775807LL - 1)
#error "LLONG_MIN is above -9223372036854775808"
#endif
#if ULLONG_MAX < 18446744073709551615ULL
#error "ULLONG_MAX is below 18446744073709551615"
#endif
#if LONG_BIT < 32
#error "LONG_BIT is below 32"
#endif
#if WORD_BIT < 32
#error "WORD_BIT is below 32"
#endif
#if MB_LEN_MAX < 1
#error "MB_LEN_MAX is below 1"
#endif

int main(void)
{
	pc_begin("XBD.limits.h.007");
	pc_test();
	pc_pass();
}
