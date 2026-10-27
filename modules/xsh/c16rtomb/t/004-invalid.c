/*
 * c32rtomb() and c16rtomb() return (size_t)-1 with EILSEQ for a code that
 * is no character; the POSIX locale has 256 characters, so among 257
 * distinct codes at least one is invalid.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.c16rtomb.004
 */
#include "pc.h"
#include <errno.h>
#include <string.h>
#include <uchar.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	char buf[32];
	size_t r;
	int v, e, bad32 = 0, bad16 = 0, wrong = -1, bade = 0;

	pc_begin("XSH.c16rtomb.004");
	pc_test();
	for (v = 0; v <= 256; v++) {
		memset(&st, 0, sizeof st);
		errno = EDOM;
		r = c32rtomb(buf, (char32_t)v, &st);
		e = errno;
		if (r == (size_t)-1) {
			bad32++;
			if (e != EILSEQ && bade == 0)
				bade = e;
		} else if (r != 1 && wrong < 0)
			wrong = v;
		memset(&st, 0, sizeof st);
		errno = EDOM;
		r = c16rtomb(buf, (char16_t)v, &st);
		e = errno;
		if (r == (size_t)-1) {
			bad16++;
			if (e != EILSEQ && bade == 0)
				bade = e;
		} else if (r != 1 && wrong < 0)
			wrong = v;
	}
	pc_check(wrong < 0, "a conversion of code %d returned neither 1 nor (size_t)-1", wrong);
	pc_check(bade == 0, "a conversion failed with errno %s, expected EILSEQ", pc_errname(bade));
	pc_check(bad32 > 0, "c32rtomb() accepted all 257 codes 0 to 256, but the locale has 256 characters");
	pc_check(bad16 > 0, "c16rtomb() accepted all 257 codes 0 to 256, but the locale has 256 characters");
	pc_pass();
}
