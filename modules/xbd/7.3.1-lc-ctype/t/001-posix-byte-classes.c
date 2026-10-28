/*
 * In the POSIX locale the classes upper, lower, alpha, digit, alnum,
 * xdigit, space and blank hold exactly the portable characters the locale
 * definition lists, among all 256 byte values: no other byte is in them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.1-lc-ctype.001
 */
#include "pc.h"
#include "pcchars.h"
#include <ctype.h>
#include <locale.h>

#define WANT(fn, flags) \
	pc_check((fn(b) != 0) == ((f & (flags)) != 0), "byte %d: %s is %s, expected %s", b, #fn, fn(b) ? "true" : "false", \
	    (f & (flags)) ? "true" : "false")

int main(void)
{
	int b;

	pc_begin("XBD.7.3.1-lc-ctype.001");
	pcc_init();
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	pc_test();
	for (b = 0; b < 256; b++) {
		unsigned f = pcc_flags[b];

		WANT(isupper, PCC_UPPER);
		WANT(islower, PCC_LOWER);
		WANT(isalpha, PCC_UPPER | PCC_LOWER);
		WANT(isdigit, PCC_DIGIT);
		WANT(isalnum, PCC_UPPER | PCC_LOWER | PCC_DIGIT);
		WANT(isxdigit, PCC_XDIGIT);
		WANT(isspace, PCC_SPACE);
		WANT(isblank, PCC_BLANK);
	}
	pc_pass();
}
