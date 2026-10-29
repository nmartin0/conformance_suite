/*
 * mbrtoc32() and mbrtoc16() accept a null pc and a null ps.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtoc16.004
 */
#include "pc.h"
#include <string.h>
#include <uchar.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st32, st16;	/* one per function */
	char32_t c32;
	char16_t c16;
	size_t a32, a16, b32, b16;

	pc_begin("XSH.mbrtoc16.004");
	memset(&st32, 0, sizeof st32);
	memset(&st16, 0, sizeof st16);
	pc_test();
	a32 = mbrtoc32(NULL, "A", 1, &st32);
	a16 = mbrtoc16(NULL, "A", 1, &st16);
	b32 = mbrtoc32(&c32, "A", 1, NULL);
	b16 = mbrtoc16(&c16, "A", 1, NULL);
	pc_check(a32 == 1 && a16 == 1, "a conversion with a null pc returned %zu and %zu, expected 1", a32, a16);
	pc_check(b32 == 1 && b16 == 1, "a conversion with a null ps returned %zu and %zu, expected 1", b32, b16);
	pc_pass();
}
