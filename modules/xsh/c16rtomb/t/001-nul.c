/*
 * c32rtomb() and c16rtomb() of the null code give a null byte and the
 * initial state.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.c16rtomb.001
 */
#include "pc.h"
#include <string.h>
#include <uchar.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	char b32[8], b16[8], n32b[8], n16b[8];
	size_t r32, r16, n32, n16;
	int i32, i16;

	pc_begin("XSH.c16rtomb.001");
	memset(&st, 0, sizeof st);
	memset(b32, 'x', sizeof b32);
	memset(b16, 'x', sizeof b16);
	memset(n32b, 'x', sizeof n32b);
	memset(n16b, 'x', sizeof n16b);
	pc_test();
	r32 = c32rtomb(b32, 0, &st);
	i32 = mbsinit(&st);
	memset(&st, 0, sizeof st);
	r16 = c16rtomb(b16, 0, &st);
	i16 = mbsinit(&st);
	n32 = c32rtomb(n32b, 0, NULL);
	n16 = c16rtomb(n16b, 0, NULL);
	pc_check(r32 == 1 && b32[0] == '\0', "c32rtomb() of the null code returned %zu and stored %d", r32, b32[0]);
	pc_check(r16 == 1 && b16[0] == '\0', "c16rtomb() of the null code returned %zu and stored %d", r16, b16[0]);
	pc_check(i32 != 0, "c32rtomb() of the null code left a state that is not initial");
	pc_check(i16 != 0, "c16rtomb() of the null code left a state that is not initial");
	pc_check(n32 == 1 && n32b[0] == '\0', "c32rtomb() with a null ps of the null code returned %zu and stored %d", n32, n32b[0]);
	pc_check(n16 == 1 && n16b[0] == '\0', "c16rtomb() with a null ps of the null code returned %zu and stored %d", n16, n16b[0]);
	pc_pass();
}
