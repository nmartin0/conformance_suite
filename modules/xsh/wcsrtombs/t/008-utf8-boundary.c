/*
 * In a UTF-8 locale wcsrtombs() produces the multibyte form and never
 * stores part of a character: it stops before the character that would
 * exceed len bytes, with *src at that character.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsrtombs.008
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
	char d[32];
	size_t n, r, len;
	static const size_t lens[3] = { 2, 3, 5 };
	static const size_t wantr[3] = { 1, 3, 3 };
	static const int wantoff[3] = { 1, 2, 2 };
	int i;

	pc_begin("XSH.wcsrtombs.008");
	use_utf8();
	n = mbstowcs(w, mb, 8);
	pc_check(n == 3, "setup: mbstowcs() returned %zu, expected 3", n);
	pc_test();
	memset(&st, 0, sizeof st);
	src = w;
	memset(d, 'x', sizeof d);
	r = wcsrtombs(d, &src, sizeof d, &st);
	pc_check(r == 6 && memcmp(d, mb, 7) == 0, "wcsrtombs() returned %zu or stored other bytes", r);
	for (i = 0; i < 3; i++) {
		len = lens[i];
		memset(&st, 0, sizeof st);
		src = w;
		memset(d, 'x', sizeof d);
		r = wcsrtombs(d, &src, len, &st);
		pc_check(r == wantr[i], "wcsrtombs() with len %zu returned %zu, expected %zu", len, r, wantr[i]);
		pc_check(src == w + wantoff[i], "wcsrtombs() with len %zu left *src at the wrong character", len);
		pc_check(memcmp(d, mb, wantr[i]) == 0, "wcsrtombs() with len %zu stored the wrong bytes", len);
		pc_check(d[len] == 'x', "wcsrtombs() with len %zu modified a byte beyond len", len);
	}
	pc_pass();
}
