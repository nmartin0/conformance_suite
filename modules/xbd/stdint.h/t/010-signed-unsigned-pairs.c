/*
 * Signed and unsigned counterparts come together.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdint.h.010
 */
#include <stdint.h>
#include <limits.h>
#include "pc.h"

#if defined(INT8_MAX) != defined(UINT8_MAX) || defined(INT8_MAX) != defined(INT8_MIN)
#error "INT8_MAX, INT8_MIN and UINT8_MAX are not all defined or all absent"
#endif
#if defined(INT16_MAX) != defined(UINT16_MAX) || defined(INT16_MAX) != defined(INT16_MIN)
#error "INT16_MAX, INT16_MIN and UINT16_MAX are not all defined or all absent"
#endif
#if defined(INT32_MAX) != defined(UINT32_MAX) || defined(INT32_MAX) != defined(INT32_MIN)
#error "INT32_MAX, INT32_MIN and UINT32_MAX are not all defined or all absent"
#endif
#if defined(INT64_MAX) != defined(UINT64_MAX) || defined(INT64_MAX) != defined(INT64_MIN)
#error "INT64_MAX, INT64_MIN and UINT64_MAX are not all defined or all absent"
#endif
#if defined(INT_LEAST8_MAX) != defined(UINT_LEAST8_MAX) || defined(INT_LEAST8_MAX) != defined(INT_LEAST8_MIN)
#error "INT_LEAST8_MAX, INT_LEAST8_MIN and UINT_LEAST8_MAX are not all defined or all absent"
#endif
#if defined(INT_LEAST16_MAX) != defined(UINT_LEAST16_MAX) || defined(INT_LEAST16_MAX) != defined(INT_LEAST16_MIN)
#error "INT_LEAST16_MAX, INT_LEAST16_MIN and UINT_LEAST16_MAX are not all defined or all absent"
#endif
#if defined(INT_LEAST32_MAX) != defined(UINT_LEAST32_MAX) || defined(INT_LEAST32_MAX) != defined(INT_LEAST32_MIN)
#error "INT_LEAST32_MAX, INT_LEAST32_MIN and UINT_LEAST32_MAX are not all defined or all absent"
#endif
#if defined(INT_LEAST64_MAX) != defined(UINT_LEAST64_MAX) || defined(INT_LEAST64_MAX) != defined(INT_LEAST64_MIN)
#error "INT_LEAST64_MAX, INT_LEAST64_MIN and UINT_LEAST64_MAX are not all defined or all absent"
#endif
#if defined(INT_FAST8_MAX) != defined(UINT_FAST8_MAX) || defined(INT_FAST8_MAX) != defined(INT_FAST8_MIN)
#error "INT_FAST8_MAX, INT_FAST8_MIN and UINT_FAST8_MAX are not all defined or all absent"
#endif
#if defined(INT_FAST16_MAX) != defined(UINT_FAST16_MAX) || defined(INT_FAST16_MAX) != defined(INT_FAST16_MIN)
#error "INT_FAST16_MAX, INT_FAST16_MIN and UINT_FAST16_MAX are not all defined or all absent"
#endif
#if defined(INT_FAST32_MAX) != defined(UINT_FAST32_MAX) || defined(INT_FAST32_MAX) != defined(INT_FAST32_MIN)
#error "INT_FAST32_MAX, INT_FAST32_MIN and UINT_FAST32_MAX are not all defined or all absent"
#endif
#if defined(INT_FAST64_MAX) != defined(UINT_FAST64_MAX) || defined(INT_FAST64_MAX) != defined(INT_FAST64_MIN)
#error "INT_FAST64_MAX, INT_FAST64_MIN and UINT_FAST64_MAX are not all defined or all absent"
#endif
#if defined(INTMAX_MAX) != defined(UINTMAX_MAX) || defined(INTPTR_MAX) != defined(UINTPTR_MAX)
#error "a signed and an unsigned limit are not provided together"
#endif

_Static_assert(sizeof(int8_t) == sizeof(uint8_t), "int8_t and uint8_t differ in size");
_Static_assert(sizeof(int16_t) == sizeof(uint16_t), "int16_t and uint16_t differ in size");
_Static_assert(sizeof(int32_t) == sizeof(uint32_t), "int32_t and uint32_t differ in size");
_Static_assert(sizeof(int_least8_t) == sizeof(uint_least8_t), "int_least8_t and uint_least8_t differ in size");
_Static_assert(sizeof(int_fast8_t) == sizeof(uint_fast8_t), "int_fast8_t and uint_fast8_t differ in size");
_Static_assert(sizeof(int_least16_t) == sizeof(uint_least16_t), "int_least16_t and uint_least16_t differ in size");
_Static_assert(sizeof(int_fast16_t) == sizeof(uint_fast16_t), "int_fast16_t and uint_fast16_t differ in size");
_Static_assert(sizeof(int_least32_t) == sizeof(uint_least32_t), "int_least32_t and uint_least32_t differ in size");
_Static_assert(sizeof(int_fast32_t) == sizeof(uint_fast32_t), "int_fast32_t and uint_fast32_t differ in size");
_Static_assert(sizeof(int_least64_t) == sizeof(uint_least64_t), "int_least64_t and uint_least64_t differ in size");
_Static_assert(sizeof(int_fast64_t) == sizeof(uint_fast64_t), "int_fast64_t and uint_fast64_t differ in size");
_Static_assert(sizeof(intmax_t) == sizeof(uintmax_t), "intmax_t and uintmax_t differ in size");
#ifdef INT64_MAX
_Static_assert(sizeof(int64_t) == sizeof(uint64_t), "int64_t and uint64_t differ in size");
#endif
#ifdef INTPTR_MAX
_Static_assert(sizeof(intptr_t) == sizeof(uintptr_t), "intptr_t and uintptr_t differ in size");
#endif

int main(void)
{
	pc_begin("XBD.stdint.h.010");
	pc_test();
	pc_pass();
}
