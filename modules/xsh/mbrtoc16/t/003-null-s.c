/*
 * mbrtoc32() and mbrtoc16() with a null s act as a conversion of the null
 * byte: they return 0, store nothing and leave the initial state, whatever
 * pc and n are.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtoc16.003
 */
#include "pc.h"
#include <string.h>
#include <uchar.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st32, st16;	/* one per function */
	char32_t c32 = 77;
	char16_t c16 = 77;
	size_t r32, r16;
	int i32, i16;

	pc_begin("XSH.mbrtoc16.003");
	memset(&st32, 0, sizeof st32);
	memset(&st16, 0, sizeof st16);
	pc_test();
	r32 = mbrtoc32(&c32, NULL, 5, &st32);
	i32 = mbsinit(&st32);
	r16 = mbrtoc16(&c16, NULL, 0, &st16);
	i16 = mbsinit(&st16);
	pc_check(r32 == 0, "mbrtoc32(&c, NULL, 5, &st) returned %zu, expected 0", r32);
	pc_check(r16 == 0, "mbrtoc16(&c, NULL, 0, &st) returned %zu, expected 0", r16);
	pc_check(c32 == 77 && c16 == 77, "a conversion with a null s stored into *pc");
	pc_check(i32 != 0 && i16 != 0, "a conversion with a null s left a state that is not initial");
	pc_pass();
}
