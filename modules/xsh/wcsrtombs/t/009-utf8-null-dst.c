/*
 * In a UTF-8 locale wcsrtombs() and wcsnrtombs() with a null dst count
 * bytes, not characters.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsrtombs.009
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
	static const char mb[] = "a\xC3\xA9\xE2\x82\xAC";
	mbstate_t st;
	const wchar_t *src;
	wchar_t w[8];
	size_t n, r1, r2;

	pc_begin("XSH.wcsrtombs.009");
	use_utf8();
	n = mbstowcs(w, mb, 8);
	pc_check(n == 3, "setup: mbstowcs() returned %zu, expected 3", n);
	pc_test();
	memset(&st, 0, sizeof st);
	src = w;
	r1 = wcsrtombs(NULL, &src, 0, &st);
	memset(&st, 0, sizeof st);
	src = w;
	r2 = wcsnrtombs(NULL, &src, 2, 0, &st);
	pc_check(r1 == 6, "wcsrtombs() with a null dst returned %zu, expected 6", r1);
	pc_check(r2 == 3, "wcsnrtombs() with a null dst and nwc 2 returned %zu, expected 3", r2);
	pc_pass();
}
