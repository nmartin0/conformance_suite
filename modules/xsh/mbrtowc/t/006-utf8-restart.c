/*
 * mbrtowc() returns (size_t)-2 without storing when the bytes given are an
 * incomplete but possibly valid character; calls with the rest complete it
 * and store the same code as a single call would.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtowc.006
 */
#include "pc.h"
#include <locale.h>
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
	mbstate_t st;
	wchar_t whole = 0, wc;
	size_t n, r1, r2, r3, s1, s2;

	pc_begin("XSH.mbrtowc.006");
	use_utf8();
	memset(&st, 0, sizeof st);
	n = mbrtowc(&whole, "\xE2\x82\xAC", 3, &st);
	pc_check(n == 3, "setup: mbrtowc() of a three-byte character returned %zu", n);
	pc_test();
	memset(&st, 0, sizeof st);
	wc = 77;
	r1 = mbrtowc(&wc, "\xE2", 1, &st);
	pc_check(r1 == (size_t)-2, "first byte: mbrtowc() returned %zu, expected (size_t)-2", r1);
	pc_check(wc == 77, "mbrtowc() stored a value for an incomplete character");
	r2 = mbrtowc(&wc, "\x82", 1, &st);
	pc_check(r2 == (size_t)-2, "second byte: mbrtowc() returned %zu, expected (size_t)-2", r2);
	pc_check(wc == 77, "mbrtowc() stored a value for an incomplete character");
	r3 = mbrtowc(&wc, "\xAC", 1, &st);
	pc_check(r3 == 1, "last byte: mbrtowc() returned %zu, expected 1", r3);
	pc_check(wc == whole, "the code stored after three calls differs from the code of one call");
	memset(&st, 0, sizeof st);
	wc = 77;
	s1 = mbrtowc(&wc, "\xE2\x82", 2, &st);
	pc_check(s1 == (size_t)-2 && wc == 77, "two of three bytes: mbrtowc() returned %zu or stored a value", s1);
	s2 = mbrtowc(&wc, "\xAC", 1, &st);
	pc_check(s2 == 1 && wc == whole, "remaining byte: mbrtowc() returned %zu or stored another code", s2);
	pc_pass();
}
