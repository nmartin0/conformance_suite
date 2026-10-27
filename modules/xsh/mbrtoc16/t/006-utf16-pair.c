/*
 * mbrtoc16() delivers a character outside the Basic Multilingual Plane as
 * two codes: the first call consumes the bytes, the next returns
 * (size_t)-3 with the second code and consumes none.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtoc16.006
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
	pc_begin("XSH.mbrtoc16.006");
	pc_notinuse("char16_t is not announced as UTF-16");
#else
	mbstate_t st;
	char16_t c1 = 77, c2 = 77, c3 = 77;
	size_t r1, r2, r3;

	pc_begin("XSH.mbrtoc16.006");
	use_utf8();
	memset(&st, 0, sizeof st);
	pc_test();
	r1 = mbrtoc16(&c1, "\xF0\x9F\x98\x80", 4, &st);
	r2 = mbrtoc16(&c2, "x", 1, &st);
	r3 = mbrtoc16(&c3, "x", 1, &st);
	pc_check(r1 == 4, "first call returned %zu, expected 4", r1);
	pc_check(c1 == 0xD83D, "first call stored 0x%lX, expected 0xD83D", (unsigned long)c1);
	pc_check(r2 == (size_t)-3, "second call returned %zu, expected (size_t)-3", r2);
	pc_check(c2 == 0xDE00, "second call stored 0x%lX, expected 0xDE00", (unsigned long)c2);
	pc_check(r3 == 1, "third call returned %zu, expected 1 (the byte it was given)", r3);
	pc_pass();
#endif
}
