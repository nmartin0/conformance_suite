/*
 * The same membership holds for the wide-character functions and for
 * iswctype() with the class names, over the wide-character codes of all
 * 256 characters of the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.1-lc-ctype.002
 */
#include "pc.h"
#include "pcchars.h"
#include <locale.h>
#include <wchar.h>
#include <wctype.h>

#define WANT(fn, flags) \
	pc_check((fn(w) != 0) == ((f & (flags)) != 0), "byte %d: %s is %s, expected %s", b, #fn, fn(w) ? "true" : "false", \
	    (f & (flags)) ? "true" : "false")
#define WANTT(name, flags) \
	pc_check((iswctype(w, wctype(name)) != 0) == ((f & (flags)) != 0), "byte %d: iswctype(%s) is %s, expected %s", b, name, \
	    iswctype(w, wctype(name)) ? "true" : "false", (f & (flags)) ? "true" : "false")

int main(void)
{
	int b;

	pc_begin("XBD.7.3.1-lc-ctype.002");
	pcc_init();
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	pc_test();
	for (b = 0; b < 256; b++) {
		unsigned f = pcc_flags[b];
		wint_t w = btowc(b);

		pc_check(w != WEOF, "byte %d is not a character of the POSIX locale", b);
		WANT(iswupper, PCC_UPPER);
		WANT(iswlower, PCC_LOWER);
		WANT(iswalpha, PCC_UPPER | PCC_LOWER);
		WANT(iswdigit, PCC_DIGIT);
		WANT(iswalnum, PCC_UPPER | PCC_LOWER | PCC_DIGIT);
		WANT(iswxdigit, PCC_XDIGIT);
		WANT(iswspace, PCC_SPACE);
		WANT(iswblank, PCC_BLANK);
		WANTT("upper", PCC_UPPER);
		WANTT("lower", PCC_LOWER);
		WANTT("alpha", PCC_UPPER | PCC_LOWER);
		WANTT("digit", PCC_DIGIT);
		WANTT("alnum", PCC_UPPER | PCC_LOWER | PCC_DIGIT);
		WANTT("xdigit", PCC_XDIGIT);
		WANTT("space", PCC_SPACE);
		WANTT("blank", PCC_BLANK);
	}
	pc_pass();
}
