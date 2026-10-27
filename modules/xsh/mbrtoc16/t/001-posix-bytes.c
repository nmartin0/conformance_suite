/*
 * In the POSIX locale mbrtoc32() and mbrtoc16() convert every non-null byte
 * as a one-byte character; the null byte gives 0, the null code and the
 * initial state.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtoc16.001
 */
#include "pc.h"
#include <string.h>
#include <uchar.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	char32_t c32 = 77;
	char16_t c16 = 77;
	char c;
	size_t r, r32 = 99, r16 = 99, bad32 = 0, bad16 = 0;
	int b, badb32 = -1, badb16 = -1, init32, init16;

	pc_begin("XSH.mbrtoc16.001");
	pc_test();
	memset(&st, 0, sizeof st);
	for (b = 1; b <= 255; b++) {
		c = (char)b;
		r = mbrtoc32(&c32, &c, 1, &st);
		if (r != 1 && badb32 < 0) {
			badb32 = b;
			bad32 = r;
		}
	}
	memset(&st, 0, sizeof st);
	for (b = 1; b <= 255; b++) {
		c = (char)b;
		r = mbrtoc16(&c16, &c, 1, &st);
		if (r != 1 && badb16 < 0) {
			badb16 = b;
			bad16 = r;
		}
	}
	pc_check(badb32 < 0, "mbrtoc32() of byte %d returned %zu, expected 1", badb32, bad32);
	pc_check(badb16 < 0, "mbrtoc16() of byte %d returned %zu, expected 1", badb16, bad16);
	memset(&st, 0, sizeof st);
	c32 = 77;
	r32 = mbrtoc32(&c32, "", 1, &st);
	init32 = mbsinit(&st);
	memset(&st, 0, sizeof st);
	c16 = 77;
	r16 = mbrtoc16(&c16, "", 1, &st);
	init16 = mbsinit(&st);
	pc_check(r32 == 0 && c32 == 0, "mbrtoc32() of the null byte returned %zu and stored %ld", r32, (long)c32);
	pc_check(r16 == 0 && c16 == 0, "mbrtoc16() of the null byte returned %zu and stored %ld", r16, (long)c16);
	pc_check(init32 != 0 && init16 != 0, "a conversion of the null byte left a state that is not initial");
	pc_pass();
}
