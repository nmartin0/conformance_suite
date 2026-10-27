/*
 * In a UTF-8 locale c32rtomb() stores the UTF-8 form of Unicode codes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.c16rtomb.005
 */
#include "pc.h"
#include <locale.h>
#include <stdlib.h>
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
#ifndef __STDC_UTF_32__
	pc_begin("XSH.c16rtomb.005");
	pc_notinuse("char32_t is not announced as UTF-32");
#else
	static const char32_t c[3] = { 0xE9, 0x20AC, 0x1F600 };
	static const char *const s[3] = { "\xC3\xA9", "\xE2\x82\xAC", "\xF0\x9F\x98\x80" };
	static const size_t want[3] = { 2, 3, 4 };
	mbstate_t st;
	char buf[16];
	size_t r[3];
	int i;

	pc_begin("XSH.c16rtomb.005");
	use_utf8();
	pc_test();
	for (i = 0; i < 3; i++) {
		memset(&st, 0, sizeof st);
		memset(buf, 'x', sizeof buf);
		r[i] = c32rtomb(buf, c[i], &st);
		pc_check(r[i] == want[i] && memcmp(buf, s[i], want[i]) == 0, "c32rtomb() of code 0x%lX returned %zu or stored other bytes", (unsigned long)c[i], r[i]);
		pc_check(r[i] <= MB_CUR_MAX, "c32rtomb() returned %zu, more than MB_CUR_MAX", r[i]);
	}
	pc_pass();
#endif
}
