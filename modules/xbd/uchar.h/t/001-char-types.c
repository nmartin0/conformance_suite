/*
 * char16_t and char32_t are the uint_least types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.uchar.h.001
 */
#include <uchar.h>
#include <stdint.h>
#include "pc.h"

_Static_assert(_Generic((char16_t)0, uint_least16_t: 1, default: 0), "char16_t is not uint_least16_t");
_Static_assert(_Generic((char32_t)0, uint_least32_t: 1, default: 0), "char32_t is not uint_least32_t");
_Static_assert(_Generic((uint_least16_t)0, char16_t: 1, default: 0), "uint_least16_t is not char16_t");
_Static_assert(_Generic((uint_least32_t)0, char32_t: 1, default: 0), "uint_least32_t is not char32_t");

int main(void)
{
	pc_begin("XBD.uchar.h.001");
	pc_test();
	pc_pass();
}
