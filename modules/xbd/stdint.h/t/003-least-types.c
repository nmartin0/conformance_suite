/*
 * Minimum-width integer types and their limits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdint.h.003
 */
#include <stdint.h>
#include <limits.h>
#include "pc.h"
#define TYID(e) _Generic((e), char: 1, signed char: 2, unsigned char: 3, short: 4, unsigned short: 5, \
	int: 6, unsigned int: 7, long: 8, unsigned long: 9, long long: 10, unsigned long long: 11, default: 0)

#if INT_LEAST8_MAX < 127 || INT_LEAST8_MIN > (-127 - 1) || UINT_LEAST8_MAX < 255
#error "a limit of the 8-bit least types is too small in magnitude"
#endif
_Static_assert(TYID(INT_LEAST8_MAX) == TYID(+(int_least8_t)0) && TYID(INT_LEAST8_MIN) == TYID(+(int_least8_t)0) && TYID(UINT_LEAST8_MAX) == TYID(+(uint_least8_t)0),
	"a limit of the 8-bit least types does not have the promoted type");
_Static_assert((int_least8_t)-1 < 0, "int_least8_t is not signed");
_Static_assert((uint_least8_t)-1 > 0, "uint_least8_t is not unsigned");
_Static_assert(sizeof(int_least8_t) * CHAR_BIT >= 8 && sizeof(uint_least8_t) * CHAR_BIT >= 8, "a 8-bit least type is narrower than 8 bits");
#if INT_LEAST16_MAX < 32767 || INT_LEAST16_MIN > (-32767 - 1) || UINT_LEAST16_MAX < 65535
#error "a limit of the 16-bit least types is too small in magnitude"
#endif
_Static_assert(TYID(INT_LEAST16_MAX) == TYID(+(int_least16_t)0) && TYID(INT_LEAST16_MIN) == TYID(+(int_least16_t)0) && TYID(UINT_LEAST16_MAX) == TYID(+(uint_least16_t)0),
	"a limit of the 16-bit least types does not have the promoted type");
_Static_assert((int_least16_t)-1 < 0, "int_least16_t is not signed");
_Static_assert((uint_least16_t)-1 > 0, "uint_least16_t is not unsigned");
_Static_assert(sizeof(int_least16_t) * CHAR_BIT >= 16 && sizeof(uint_least16_t) * CHAR_BIT >= 16, "a 16-bit least type is narrower than 16 bits");
#if INT_LEAST32_MAX < 2147483647 || INT_LEAST32_MIN > (-2147483647 - 1) || UINT_LEAST32_MAX < 4294967295U
#error "a limit of the 32-bit least types is too small in magnitude"
#endif
_Static_assert(TYID(INT_LEAST32_MAX) == TYID(+(int_least32_t)0) && TYID(INT_LEAST32_MIN) == TYID(+(int_least32_t)0) && TYID(UINT_LEAST32_MAX) == TYID(+(uint_least32_t)0),
	"a limit of the 32-bit least types does not have the promoted type");
_Static_assert((int_least32_t)-1 < 0, "int_least32_t is not signed");
_Static_assert((uint_least32_t)-1 > 0, "uint_least32_t is not unsigned");
_Static_assert(sizeof(int_least32_t) * CHAR_BIT >= 32 && sizeof(uint_least32_t) * CHAR_BIT >= 32, "a 32-bit least type is narrower than 32 bits");
#if INT_LEAST64_MAX < 9223372036854775807LL || INT_LEAST64_MIN > (-9223372036854775807LL - 1) || UINT_LEAST64_MAX < 18446744073709551615ULL
#error "a limit of the 64-bit least types is too small in magnitude"
#endif
_Static_assert(TYID(INT_LEAST64_MAX) == TYID(+(int_least64_t)0) && TYID(INT_LEAST64_MIN) == TYID(+(int_least64_t)0) && TYID(UINT_LEAST64_MAX) == TYID(+(uint_least64_t)0),
	"a limit of the 64-bit least types does not have the promoted type");
_Static_assert((int_least64_t)-1 < 0, "int_least64_t is not signed");
_Static_assert((uint_least64_t)-1 > 0, "uint_least64_t is not unsigned");
_Static_assert(sizeof(int_least64_t) * CHAR_BIT >= 64 && sizeof(uint_least64_t) * CHAR_BIT >= 64, "a 64-bit least type is narrower than 64 bits");

/* No standard type of smaller range is wide enough: the least types are the smallest ones. */
#if SCHAR_MAX >= 127 && INT_LEAST8_MAX > SCHAR_MAX
#error "int_least8_t is wider than schar although schar is wide enough"
#endif
#if SHRT_MAX >= 127 && INT_LEAST8_MAX > SHRT_MAX
#error "int_least8_t is wider than shrt although shrt is wide enough"
#endif
#if INT_MAX >= 127 && INT_LEAST8_MAX > INT_MAX
#error "int_least8_t is wider than int although int is wide enough"
#endif
#if LONG_MAX >= 127 && INT_LEAST8_MAX > LONG_MAX
#error "int_least8_t is wider than long although long is wide enough"
#endif
#if LLONG_MAX >= 127 && INT_LEAST8_MAX > LLONG_MAX
#error "int_least8_t is wider than llong although llong is wide enough"
#endif
#if UCHAR_MAX >= 255 && UINT_LEAST8_MAX > UCHAR_MAX
#error "uint_least8_t is wider than uchar although it is wide enough"
#endif
#if USHRT_MAX >= 255 && UINT_LEAST8_MAX > USHRT_MAX
#error "uint_least8_t is wider than ushrt although it is wide enough"
#endif
#if UINT_MAX >= 255 && UINT_LEAST8_MAX > UINT_MAX
#error "uint_least8_t is wider than uint although it is wide enough"
#endif
#if ULONG_MAX >= 255 && UINT_LEAST8_MAX > ULONG_MAX
#error "uint_least8_t is wider than ulong although it is wide enough"
#endif
#if ULLONG_MAX >= 255 && UINT_LEAST8_MAX > ULLONG_MAX
#error "uint_least8_t is wider than ullong although it is wide enough"
#endif
#if SCHAR_MAX >= 32767 && INT_LEAST16_MAX > SCHAR_MAX
#error "int_least16_t is wider than schar although schar is wide enough"
#endif
#if SHRT_MAX >= 32767 && INT_LEAST16_MAX > SHRT_MAX
#error "int_least16_t is wider than shrt although shrt is wide enough"
#endif
#if INT_MAX >= 32767 && INT_LEAST16_MAX > INT_MAX
#error "int_least16_t is wider than int although int is wide enough"
#endif
#if LONG_MAX >= 32767 && INT_LEAST16_MAX > LONG_MAX
#error "int_least16_t is wider than long although long is wide enough"
#endif
#if LLONG_MAX >= 32767 && INT_LEAST16_MAX > LLONG_MAX
#error "int_least16_t is wider than llong although llong is wide enough"
#endif
#if UCHAR_MAX >= 65535 && UINT_LEAST16_MAX > UCHAR_MAX
#error "uint_least16_t is wider than uchar although it is wide enough"
#endif
#if USHRT_MAX >= 65535 && UINT_LEAST16_MAX > USHRT_MAX
#error "uint_least16_t is wider than ushrt although it is wide enough"
#endif
#if UINT_MAX >= 65535 && UINT_LEAST16_MAX > UINT_MAX
#error "uint_least16_t is wider than uint although it is wide enough"
#endif
#if ULONG_MAX >= 65535 && UINT_LEAST16_MAX > ULONG_MAX
#error "uint_least16_t is wider than ulong although it is wide enough"
#endif
#if ULLONG_MAX >= 65535 && UINT_LEAST16_MAX > ULLONG_MAX
#error "uint_least16_t is wider than ullong although it is wide enough"
#endif
#if SCHAR_MAX >= 2147483647 && INT_LEAST32_MAX > SCHAR_MAX
#error "int_least32_t is wider than schar although schar is wide enough"
#endif
#if SHRT_MAX >= 2147483647 && INT_LEAST32_MAX > SHRT_MAX
#error "int_least32_t is wider than shrt although shrt is wide enough"
#endif
#if INT_MAX >= 2147483647 && INT_LEAST32_MAX > INT_MAX
#error "int_least32_t is wider than int although int is wide enough"
#endif
#if LONG_MAX >= 2147483647 && INT_LEAST32_MAX > LONG_MAX
#error "int_least32_t is wider than long although long is wide enough"
#endif
#if LLONG_MAX >= 2147483647 && INT_LEAST32_MAX > LLONG_MAX
#error "int_least32_t is wider than llong although llong is wide enough"
#endif
#if UCHAR_MAX >= 4294967295U && UINT_LEAST32_MAX > UCHAR_MAX
#error "uint_least32_t is wider than uchar although it is wide enough"
#endif
#if USHRT_MAX >= 4294967295U && UINT_LEAST32_MAX > USHRT_MAX
#error "uint_least32_t is wider than ushrt although it is wide enough"
#endif
#if UINT_MAX >= 4294967295U && UINT_LEAST32_MAX > UINT_MAX
#error "uint_least32_t is wider than uint although it is wide enough"
#endif
#if ULONG_MAX >= 4294967295U && UINT_LEAST32_MAX > ULONG_MAX
#error "uint_least32_t is wider than ulong although it is wide enough"
#endif
#if ULLONG_MAX >= 4294967295U && UINT_LEAST32_MAX > ULLONG_MAX
#error "uint_least32_t is wider than ullong although it is wide enough"
#endif
#if SCHAR_MAX >= 9223372036854775807LL && INT_LEAST64_MAX > SCHAR_MAX
#error "int_least64_t is wider than schar although schar is wide enough"
#endif
#if SHRT_MAX >= 9223372036854775807LL && INT_LEAST64_MAX > SHRT_MAX
#error "int_least64_t is wider than shrt although shrt is wide enough"
#endif
#if INT_MAX >= 9223372036854775807LL && INT_LEAST64_MAX > INT_MAX
#error "int_least64_t is wider than int although int is wide enough"
#endif
#if LONG_MAX >= 9223372036854775807LL && INT_LEAST64_MAX > LONG_MAX
#error "int_least64_t is wider than long although long is wide enough"
#endif
#if LLONG_MAX >= 9223372036854775807LL && INT_LEAST64_MAX > LLONG_MAX
#error "int_least64_t is wider than llong although llong is wide enough"
#endif
#if UCHAR_MAX >= 18446744073709551615ULL && UINT_LEAST64_MAX > UCHAR_MAX
#error "uint_least64_t is wider than uchar although it is wide enough"
#endif
#if USHRT_MAX >= 18446744073709551615ULL && UINT_LEAST64_MAX > USHRT_MAX
#error "uint_least64_t is wider than ushrt although it is wide enough"
#endif
#if UINT_MAX >= 18446744073709551615ULL && UINT_LEAST64_MAX > UINT_MAX
#error "uint_least64_t is wider than uint although it is wide enough"
#endif
#if ULONG_MAX >= 18446744073709551615ULL && UINT_LEAST64_MAX > ULONG_MAX
#error "uint_least64_t is wider than ulong although it is wide enough"
#endif
#if ULLONG_MAX >= 18446744073709551615ULL && UINT_LEAST64_MAX > ULLONG_MAX
#error "uint_least64_t is wider than ullong although it is wide enough"
#endif

int main(void)
{
	pc_begin("XBD.stdint.h.003");
	pc_test();
	pc_pass();
}
