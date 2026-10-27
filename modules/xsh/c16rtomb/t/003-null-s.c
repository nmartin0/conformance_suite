/*
 * c32rtomb() and c16rtomb() with a null s act as a conversion of the null
 * code into an internal buffer, whatever the code is: they return 1 from
 * the initial state and leave the initial state.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.c16rtomb.003
 */
#include "pc.h"
#include <string.h>
#include <uchar.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	size_t r32, r16;
	int i32, i16;

	pc_begin("XSH.c16rtomb.003");
	memset(&st, 0, sizeof st);
	pc_test();
	r32 = c32rtomb(NULL, 0x41, &st);
	i32 = mbsinit(&st);
	memset(&st, 0, sizeof st);
	r16 = c16rtomb(NULL, 0x41, &st);
	i16 = mbsinit(&st);
	pc_check(r32 == 1, "c32rtomb(NULL, 0x41, &st) returned %zu, expected 1", r32);
	pc_check(r16 == 1, "c16rtomb(NULL, 0x41, &st) returned %zu, expected 1", r16);
	pc_check(i32 != 0 && i16 != 0, "a conversion with a null s left a state that is not initial");
	pc_pass();
}
