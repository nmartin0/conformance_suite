/*
 * In a UTF-8 locale c16rtomb() stores the UTF-8 form of Basic Multilingual
 * Plane codes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.c16rtomb.006
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
	pc_begin("XSH.c16rtomb.006");
	pc_notinuse("char16_t is not announced as UTF-16");
#else
	static const char16_t c[2] = { 0xE9, 0x20AC };
	static const char *const s[2] = { "\xC3\xA9", "\xE2\x82\xAC" };
	static const size_t want[2] = { 2, 3 };
	mbstate_t st;
	char buf[16];
	size_t r[2];
	int i;

	pc_begin("XSH.c16rtomb.006");
	use_utf8();
	pc_test();
	for (i = 0; i < 2; i++) {
		memset(&st, 0, sizeof st);
		memset(buf, 'x', sizeof buf);
		r[i] = c16rtomb(buf, c[i], &st);
		pc_check(r[i] == want[i] && memcmp(buf, s[i], want[i]) == 0, "c16rtomb() of code 0x%lX returned %zu or stored other bytes", (unsigned long)c[i], r[i]);
	}
	pc_pass();
#endif
}
