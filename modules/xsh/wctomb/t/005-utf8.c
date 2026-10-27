/*
 * In a UTF-8 locale wctomb() stores the multibyte form of codes that
 * mbtowc() produced, the null code gives a null byte, and a null s gives 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctomb.005
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
	static const char *const s[2] = { "\xC3\xA9", "\xE2\x82\xAC" };
	static const int want[2] = { 2, 3 };
	wchar_t wc[2];
	char buf[16];
	int r[2], ra, r0, rn, i, n;

	pc_begin("XSH.wctomb.005");
	use_utf8();
	for (i = 0; i < 2; i++) {
		n = mbtowc(&wc[i], s[i], 4);
		pc_check(n == want[i], "setup: mbtowc() returned %d, expected %d", n, want[i]);
	}
	pc_test();
	for (i = 0; i < 2; i++) {
		memset(buf, 'x', sizeof buf);
		r[i] = wctomb(buf, wc[i]);
		pc_check(r[i] == want[i] && memcmp(buf, s[i], (size_t)want[i]) == 0, "wctomb() of the code of string %d returned %d or stored other bytes", i, r[i]);
		pc_check(r[i] <= (int)MB_CUR_MAX, "wctomb() returned %d, more than MB_CUR_MAX", r[i]);
	}
	buf[0] = buf[1] = 'x';
	ra = wctomb(buf, L'A');
	pc_check(ra == 1 && buf[0] == 'A', "wctomb(L'A') returned %d and stored %d", ra, buf[0]);
	r0 = wctomb(buf, 0);
	pc_check(r0 == 1 && buf[0] == '\0', "wctomb(0) returned %d and stored %d", r0, buf[0]);
	rn = wctomb(NULL, 0);
	pc_check(rn == 0, "wctomb(NULL, 0) returned %d, expected 0", rn);
	pc_pass();
}
