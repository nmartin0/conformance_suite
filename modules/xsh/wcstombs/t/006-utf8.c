/*
 * In a UTF-8 locale wcstombs() converts to the multibyte form, and counts
 * bytes against n without storing a partial character.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstombs.006
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
	static const char mb[] = "a\xC3\xA9\xE2\x82\xAC";
	wchar_t w[8];
	char s[32];
	size_t r, r5, r2, rn, n;

	pc_begin("XSH.wcstombs.006");
	use_utf8();
	n = mbstowcs(w, mb, 8);
	pc_check(n == 3, "setup: mbstowcs() returned %zu, expected 3", n);
	pc_test();
	memset(s, 'x', sizeof s);
	r = wcstombs(s, w, sizeof s);
	pc_check(r == 6 && memcmp(s, mb, 7) == 0, "wcstombs() returned %zu or stored other bytes", r);
	memset(s, 'x', sizeof s);
	r5 = wcstombs(s, w, 5);
	pc_check(r5 == 3 && memcmp(s, "a\xC3\xA9", 3) == 0, "wcstombs() with n 5 returned %zu, expected 3 (whole characters only)", r5);
	pc_check(s[5] == 'x', "wcstombs() with n 5 modified a byte beyond n");
	memset(s, 'x', sizeof s);
	r2 = wcstombs(s, w, 2);
	pc_check(r2 == 1 && s[0] == 'a', "wcstombs() with n 2 returned %zu, expected 1", r2);
	pc_check(s[2] == 'x', "wcstombs() with n 2 modified a byte beyond n");
	rn = wcstombs(NULL, w, 0);
	pc_check(rn == 6, "wcstombs(NULL, w, 0) returned %zu, expected 6", rn);
	pc_pass();
}
