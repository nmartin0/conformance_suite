/*
 * mbrtoc32() returns (size_t)-2 without storing for an incomplete but
 * possibly valid character; calls with the rest complete it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtoc16.007
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
	mbstate_t st;
	char32_t c = 77;
	size_t r1, r2, r3;

	pc_begin("XSH.mbrtoc16.007");
	use_utf8();
	memset(&st, 0, sizeof st);
	pc_test();
	r1 = mbrtoc32(&c, "\xE2", 1, &st);
	pc_check(r1 == (size_t)-2, "first byte: mbrtoc32() returned %zu, expected (size_t)-2", r1);
	pc_check(c == 77, "mbrtoc32() stored a value for an incomplete character");
	r2 = mbrtoc32(&c, "\x82", 1, &st);
	pc_check(r2 == (size_t)-2, "second byte: mbrtoc32() returned %zu, expected (size_t)-2", r2);
	pc_check(c == 77, "mbrtoc32() stored a value for an incomplete character");
	r3 = mbrtoc32(&c, "\xAC", 1, &st);
	pc_check(r3 == 1, "last byte: mbrtoc32() returned %zu, expected 1", r3);
#ifdef __STDC_UTF_32__
	pc_check(c == 0x20AC, "the completed character gave 0x%lX, expected 0x20AC", (unsigned long)c);
#endif
	pc_pass();
}
