/*
 * wcrtomb writes the shortest form, exactly as the RFC's table gives it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.utf8.007
 * MUTANT wcrtomb.long
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
	unsigned char want[8], got[16];
	mbstate_t ps;
	size_t r;
	int n;

	pc_begin("CONV.utf8.007");
#ifndef __STDC_ISO_10646__
	pc_notinuse("wide characters are not documented as Unicode code points (__STDC_ISO_10646__ is not defined)");
#endif
	u8_locale();
	pc_test();
	for (cp = 0; cp <= 0x10FFFF; cp++) {
		n = u8_encode(cp, want);
		if (n == 0)
			continue;
		memset(&ps, 0, sizeof ps);
		memset(got, 0xAA, sizeof got);
		r = wcrtomb((char *)got, (wchar_t)cp, &ps);
		pc_check(r == (size_t)n, "wcrtomb(U+%04lX) wrote %zd bytes, the shortest form has %d", cp, (ssize_t)r, n);
		pc_check(memcmp(got, want, (size_t)n) == 0, "wcrtomb(U+%04lX) wrote bytes that differ from the RFC's encoding", cp);
	}
	pc_pass();
}
