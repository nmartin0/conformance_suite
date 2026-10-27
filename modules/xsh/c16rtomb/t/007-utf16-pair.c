/*
 * Two successive c16rtomb() calls with the two halves of a surrogate pair
 * store, between them, the four-byte UTF-8 form of the character.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.c16rtomb.007
 */
#include "pc.h"
#include <locale.h>
#include <string.h>
#include <uchar.h>
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
#ifndef __STDC_UTF_16__
	pc_begin("XSH.c16rtomb.007");
	pc_notinuse("char16_t is not announced as UTF-16");
#else
	mbstate_t st;
	char b1[16], b2[16], all[32];
	size_t r1, r2;

	pc_begin("XSH.c16rtomb.007");
	use_utf8();
	memset(&st, 0, sizeof st);
	memset(b1, 'x', sizeof b1);
	memset(b2, 'x', sizeof b2);
	pc_test();
	r1 = c16rtomb(b1, 0xD83D, &st);
	pc_check(r1 != (size_t)-1, "c16rtomb() of the high half of a surrogate pair failed");
	r2 = c16rtomb(b2, 0xDE00, &st);
	pc_check(r2 != (size_t)-1, "c16rtomb() of the low half of a surrogate pair failed");
	pc_check(r1 + r2 == 4, "the two calls returned %zu and %zu bytes, expected 4 in all", r1, r2);
	memcpy(all, b1, r1);
	memcpy(all + r1, b2, r2);
	pc_check(memcmp(all, "\xF0\x9F\x98\x80", 4) == 0, "the bytes stored by the two calls are not the UTF-8 form of U+1F600");
	pc_pass();
#endif
}
