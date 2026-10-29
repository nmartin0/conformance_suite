/*
 * mbrtowc decodes every well-formed UTF-8 sequence.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.utf8.001
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pc.h"
#include "../../../../lib/cv.h"
#include "../../../../lib/u8.h"

int main(void)
{
	unsigned long cp;
	unsigned char b[8];
	wchar_t wc;
	size_t len;
	int n;

	pc_begin("CONV.utf8.001");
	u8_locale();
	pc_test();
	for (cp = 0; cp <= 0x10FFFF; cp++) {
		n = u8_encode(cp, b);
		if (n == 0)
			continue;
		b[n] = 'x';	/* more bytes available than the character uses */
		pc_check(u8_decodes(b, (size_t)n + 1, &wc, &len) || (cp == 0 && len == 0),
		    "U+%04lX (%d bytes) is rejected by mbrtowc", cp, n);
		pc_check(cp == 0 ? len == 0 : len == (size_t)n, "U+%04lX: mbrtowc consumed %zu bytes, not %d", cp, len, n);
#ifdef __STDC_ISO_10646__
		pc_check((unsigned long)wc == cp, "U+%04lX decoded as wide character %lX", cp, (unsigned long)wc);
#endif
	}
	pc_pass();
}
