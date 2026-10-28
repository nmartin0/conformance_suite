/*
 * In the POSIX locale toupper(), tolower(), towupper() and towlower() map
 * the 26 lowercase letters to the 26 uppercase letters and back and change
 * nothing else, among all 256 byte values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.1-lc-ctype.004
 */
#include "pc.h"
#include "pcchars.h"
#include <ctype.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>

int main(void)
{
	int b;
	const char *p;

	pc_begin("XBD.7.3.1-lc-ctype.004");
	pcc_init();
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	pc_test();
	for (b = 0; b < 256; b++) {
		int up = b, lo = b;
		wint_t w = btowc(b);

		p = strchr(pcc_lower, b);
		if (p != NULL && b != 0)
			up = (unsigned char)pcc_upper[p - pcc_lower];
		p = strchr(pcc_upper, b);
		if (p != NULL && b != 0)
			lo = (unsigned char)pcc_lower[p - pcc_upper];
		pc_check(toupper(b) == up, "toupper(%d) is %d, expected %d", b, toupper(b), up);
		pc_check(tolower(b) == lo, "tolower(%d) is %d, expected %d", b, tolower(b), lo);
		pc_check(w != WEOF, "byte %d is not a character of the POSIX locale", b);
		pc_check(towupper(w) == btowc(up), "towupper of the code of byte %d is not that of byte %d", b, up);
		pc_check(towlower(w) == btowc(lo), "towlower of the code of byte %d is not that of byte %d", b, lo);
	}
	pc_pass();
}
