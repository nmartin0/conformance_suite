/*
 * mbrtowc rejects values above U+10FFFF and lead bytes that no valid sequence uses.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.utf8.004
 * MUTANT mbrtowc.above
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
	unsigned a, b, c;

	pc_begin("CONV.utf8.004");
	u8_locale();
	pc_test();
	for (a = 0x90; a <= 0xBF; a++)		/* F4 90 .. to F4 BF ..: U+110000 and up */
		for (b = 0x80; b <= 0xBF; b++)
			for (c = 0x80; c <= 0xBF; c += 0x3F) {
				s[0] = 0xF4; s[1] = (unsigned char)a; s[2] = (unsigned char)b; s[3] = (unsigned char)c;
				pc_check(!u8_decodes(s, 4, &wc, &len), "F4 %02X %02X %02X (above U+10FFFF) is decoded", a, b, c);
			}
	for (a = 0xF5; a <= 0xFF; a++)		/* lead bytes beyond F4, followed by tails */
		for (b = 0x80; b <= 0xBF; b += 0x3F) {
			memset(s, (int)b, sizeof s);
			s[0] = (unsigned char)a;
			pc_check(!u8_decodes(s, 4, &wc, &len), "lead byte %02X with tails is decoded", a);
			pc_check(!u8_decodes(s, 6, &wc, &len), "lead byte %02X with five tails is decoded", a);
		}
	/* the five- and six-octet forms of the original definition, with values above U+10FFFF */
	memcpy(s, "\xF8\x88\x80\x80\x80", 5);
	pc_check(!u8_decodes(s, 5, &wc, &len), "F8 88 80 80 80 (U+200000) is decoded");
	memcpy(s, "\xFB\xBF\xBF\xBF\xBF", 5);
	pc_check(!u8_decodes(s, 5, &wc, &len), "FB BF BF BF BF (U+3FFFFFF) is decoded");
	memcpy(s, "\xFC\x84\x80\x80\x80\x80", 6);
	pc_check(!u8_decodes(s, 6, &wc, &len), "FC 84 80 80 80 80 (U+4000000) is decoded");
	memcpy(s, "\xFD\xBF\xBF\xBF\xBF\xBF", 6);
	pc_check(!u8_decodes(s, 6, &wc, &len), "FD BF BF BF BF BF (U+7FFFFFFF) is decoded");
	pc_pass();
}
