/*
 * mbrtowc does not decode a continuation byte standing alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.utf8.006
 * MUTANT mbrtowc.stray
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
	unsigned a;

	pc_begin("CONV.utf8.006");
	u8_locale();
	pc_test();
	for (a = 0x80; a <= 0xBF; a++) {
		memset(s, 'A', sizeof s);
		s[0] = (unsigned char)a;
		pc_check(!u8_decodes(s, 1, &wc, &len), "lone continuation byte %02X is decoded", a);
		pc_check(!u8_decodes(s, 4, &wc, &len), "continuation byte %02X before AAA is decoded", a);
	}
	/* after a complete character, a continuation byte is still not a character */
	memcpy(s, "\xC3\xA9\x80", 3);
	pc_check(u8_decodes(s, 2, &wc, &len) && len == 2, "setup: U+00E9 is not decoded");
	pc_check(!u8_decodes(s + 2, 1, &wc, &len), "the continuation byte after U+00E9 is decoded");
	pc_pass();
}
