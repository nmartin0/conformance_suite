/*
 * mbrtowc rejects the surrogate range.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.utf8.003
 * MUTANT mbrtowc.surrogate
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pc.h"
#include "../../../../lib/cv.h"
#include "../../../../lib/u8.h"

int main(void)
{
	unsigned char s[8];
	wchar_t wc;
	size_t len;
	unsigned a, b;

	pc_begin("CONV.utf8.003");
	u8_locale();
	pc_test();
	for (a = 0xA0; a <= 0xBF; a++)		/* ED A0 80 to ED BF BF: U+D800 to U+DFFF */
		for (b = 0x80; b <= 0xBF; b++) {
			s[0] = 0xED; s[1] = (unsigned char)a; s[2] = (unsigned char)b;
			pc_check(!u8_decodes(s, 3, &wc, &len), "surrogate ED %02X %02X is decoded", a, b);
		}
	/* the RFC's example of a surrogate pair written as two three-octet sequences */
	memcpy(s, "\xED\xA1\x8C\xED\xBE\xB4", 6);
	pc_check(!u8_decodes(s, 6, &wc, &len), "ED A1 8C ED BE B4 is decoded (the RFC's CESU-8 style surrogate pair)");
	pc_pass();
}
