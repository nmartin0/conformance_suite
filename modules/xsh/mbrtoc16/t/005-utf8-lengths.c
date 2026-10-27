/*
 * In a UTF-8 locale mbrtoc32() returns the length of two-, three- and
 * four-byte characters, and stores their Unicode codes (when the type holds
 * UTF-32).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtoc16.005
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
	static const char *const s[3] = { "\xC3\xA9", "\xE2\x82\xAC", "\xF0\x9F\x98\x80" };
	static const size_t want[3] = { 2, 3, 4 };
	static const unsigned long code[3] = { 0xE9, 0x20AC, 0x1F600 };
	mbstate_t st;
	char32_t c;
	size_t r;
	int i;

	pc_begin("XSH.mbrtoc16.005");
	use_utf8();
	pc_test();
	for (i = 0; i < 3; i++) {
		memset(&st, 0, sizeof st);
		c = 77;
		r = mbrtoc32(&c, s[i], 4, &st);
		pc_check(r == want[i], "mbrtoc32() of string %d returned %zu, expected %zu", i, r, want[i]);
#ifdef __STDC_UTF_32__
		pc_check(c == code[i], "mbrtoc32() of string %d stored 0x%lX, expected 0x%lX", i, (unsigned long)c, code[i]);
#else
		(void)code;
#endif
	}
	pc_pass();
}
