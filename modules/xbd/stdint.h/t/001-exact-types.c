/*
 * Widths and signedness of the exact-width integer types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdint.h.001
 */
#include <stdint.h>
#include <limits.h>
#include "pc.h"

_Static_assert(sizeof(int8_t) * CHAR_BIT == 8, "int8_t is not 8 bits wide");
_Static_assert(sizeof(uint8_t) * CHAR_BIT == 8, "uint8_t is not 8 bits wide");
_Static_assert((int8_t)-1 < 0, "int8_t is not signed");
_Static_assert((uint8_t)-1 > 0, "uint8_t is not unsigned");
_Static_assert(sizeof(int16_t) * CHAR_BIT == 16, "int16_t is not 16 bits wide");
_Static_assert(sizeof(uint16_t) * CHAR_BIT == 16, "uint16_t is not 16 bits wide");
_Static_assert((int16_t)-1 < 0, "int16_t is not signed");
_Static_assert((uint16_t)-1 > 0, "uint16_t is not unsigned");
_Static_assert(sizeof(int32_t) * CHAR_BIT == 32, "int32_t is not 32 bits wide");
_Static_assert(sizeof(uint32_t) * CHAR_BIT == 32, "uint32_t is not 32 bits wide");
_Static_assert((int32_t)-1 < 0, "int32_t is not signed");
_Static_assert((uint32_t)-1 > 0, "uint32_t is not unsigned");

#ifdef INT64_MAX
_Static_assert(sizeof(int64_t) * CHAR_BIT == 64, "int64_t is not 64 bits wide");
_Static_assert(sizeof(uint64_t) * CHAR_BIT == 64, "uint64_t is not 64 bits wide");
_Static_assert((int64_t)-1 < 0, "int64_t is not signed");
_Static_assert((uint64_t)-1 > 0, "uint64_t is not unsigned");
#endif

/* A standard type with an exact width of 8, 16, 32 or 64 bits has a matching exact-width type. */
#if SCHAR_MAX == 127 && !defined(INT8_MAX)
#error "signed char is 8 bits wide but int8_t is not provided"
#endif
#if SHRT_MAX == 32767 && !defined(INT16_MAX)
#error "short is 16 bits wide but int16_t is not provided"
#endif
#if (INT_MAX == 2147483647 || SHRT_MAX == 2147483647 || LONG_MAX == 2147483647L) && !defined(INT32_MAX)
#error "a standard type is 32 bits wide but int32_t is not provided"
#endif
#if (LONG_MAX == 9223372036854775807L || LLONG_MAX == 9223372036854775807LL || INT_MAX == 9223372036854775807L) && !defined(INT64_MAX)
#error "a standard type is 64 bits wide but int64_t is not provided"
#endif

int main(void)
{
	pc_begin("XBD.stdint.h.001");
	pc_test();
	pc_pass();
}
