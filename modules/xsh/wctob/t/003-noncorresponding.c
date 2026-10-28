/*
 * wctob() returns EOF for WEOF and for a code that is not a single-byte
 * character; the POSIX locale has only 256 characters, so among 257
 * distinct codes at least one has no character.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctob.003
 */
#include "pc.h"
#include <stdio.h>
#include <wchar.h>

int main(void)
{
	wint_t v;
	int r, nof = 0, badv = -1, badr = 0;

	pc_begin("XSH.wctob.003");
	pc_test();
	r = wctob(WEOF);
	pc_check(r == EOF, "wctob(WEOF) returned %d, expected EOF", r);
	for (v = 0; v <= 256; v++) {
		r = wctob(v);
		if (r == EOF)
			nof++;
		else if (r < 0 || r > 255) {
			badv = (int)v;
			badr = r;
		}
	}
	pc_check(badv < 0, "wctob(%d) returned %d, neither EOF nor an unsigned char value", badv, badr);
	pc_check(nof > 0, "wctob() accepted all 257 codes 0 to 256, but the locale has 256 characters");
	/* the codes of the portable characters do correspond to a byte, so not every result is EOF */
	pc_check(wctob(L'a') == 'a' && wctob(L'0') == '0', "wctob() of the code of a portable character did not give its byte");
	pc_pass();
}
