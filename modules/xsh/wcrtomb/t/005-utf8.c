/*
 * In a UTF-8 locale wcrtomb() stores the multibyte form of codes that
 * mbrtowc() produced; the null wide character gives a null byte and the
 * initial state.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcrtomb.005
 */
#include "pc.h"
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

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
	static const size_t want[2] = { 2, 3 };
	mbstate_t st;
	wchar_t wc[2];
	char buf[16];
	size_t r[2], ra, r0, n;
	int i, init;

	pc_begin("XSH.wcrtomb.005");
	use_utf8();
	for (i = 0; i < 2; i++) {
		memset(&st, 0, sizeof st);
		n = mbrtowc(&wc[i], s[i], 4, &st);
		pc_check(n == want[i], "setup: mbrtowc() returned %zu, expected %zu", n, want[i]);
	}
	pc_test();
	for (i = 0; i < 2; i++) {
		memset(&st, 0, sizeof st);
		memset(buf, 'x', sizeof buf);
		r[i] = wcrtomb(buf, wc[i], &st);
		pc_check(r[i] == want[i] && memcmp(buf, s[i], want[i]) == 0, "wcrtomb() of the code of string %d returned %zu or stored other bytes", i, r[i]);
		pc_check(r[i] <= MB_CUR_MAX, "wcrtomb() returned %zu, more than MB_CUR_MAX", r[i]);
	}
	memset(&st, 0, sizeof st);
	buf[0] = buf[1] = 'x';
	ra = wcrtomb(buf, L'A', &st);
	pc_check(ra == 1 && buf[0] == 'A', "wcrtomb(L'A') returned %zu and stored %d", ra, buf[0]);
	buf[0] = 'x';
	r0 = wcrtomb(buf, 0, &st);
	init = mbsinit(&st);
	pc_check(r0 == 1 && buf[0] == '\0', "wcrtomb(0) returned %zu and stored %d", r0, buf[0]);
	pc_check(init != 0, "wcrtomb(0) left a state that is not initial");
	pc_pass();
}
