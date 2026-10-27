/*
 * In a UTF-8 locale mbtowc() reports the length of one-, two- and
 * three-byte characters, stores the value for ASCII, and wctomb() turns the
 * stored codes back into the same bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbtowc.004
 */
#include "pc.h"
#include <locale.h>
#include <stdlib.h>
#include <string.h>

static void use_utf8(void)
{
	const char *n = pc_find_locale("UTF-8", 1);

	if (n == NULL)
		pc_notinuse("no UTF-8 locale installed");
	if (setlocale(LC_ALL, n) == NULL)
		pc_unresolved("setlocale(LC_ALL, \"%s\") failed", n);
}

int main(void)
{
	static const char *const s[3] = { "A", "\xC3\xA9", "\xE2\x82\xAC" };
	static const int want[3] = { 1, 2, 3 };
	wchar_t wc[3];
	char buf[16];
	int r[3], w[3], rn, i;

	pc_begin("XSH.mbtowc.004");
	use_utf8();
	pc_test();
	rn = mbtowc(NULL, NULL, 0);
	for (i = 0; i < 3; i++) {
		wc[i] = 0;
		r[i] = mbtowc(&wc[i], s[i], 4);
		pc_check(r[i] == want[i], "mbtowc() of string %d returned %d, expected %d", i, r[i], want[i]);
		pc_check(r[i] <= (int)MB_CUR_MAX, "mbtowc() returned %d, more than MB_CUR_MAX", r[i]);
	}
	pc_check(rn == 0, "mbtowc(NULL, NULL, 0) returned %d, expected 0", rn);
	pc_check(wc[0] == (wchar_t)'A', "mbtowc() of \"A\" stored %ld", (long)wc[0]);
	for (i = 0; i < 3; i++) {
		memset(buf, 'x', sizeof buf);
		w[i] = wctomb(buf, wc[i]);
		pc_check(w[i] == want[i] && memcmp(buf, s[i], (size_t)want[i]) == 0, "wctomb() of the code stored for string %d returned %d or other bytes", i, w[i]);
	}
	pc_pass();
}
