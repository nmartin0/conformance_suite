/*
 * In a UTF-8 locale wctob() maps ASCII to its byte and refuses a character
 * that takes two bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctob.005
 */
#include "pc.h"
#include <locale.h>
#include <stdio.h>
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
	wchar_t wc = 0;
	mbstate_t st;
	size_t n;
	int r;

	pc_begin("XSH.wctob.005");
	use_utf8();
	st = (mbstate_t){ 0 };
	n = mbrtowc(&wc, "\xC3\xA9", 2, &st);
	pc_check(n == 2, "setup: mbrtowc() of a two-byte character returned %zu", n);
	pc_test();
	r = wctob((wint_t)L'A');
	pc_check(r == 'A', "wctob(L'A') returned %d", r);
	r = wctob((wint_t)wc);
	pc_check(r == EOF, "wctob() of a two-byte character returned %d, expected EOF", r);
	pc_pass();
}
