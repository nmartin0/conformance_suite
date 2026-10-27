/*
 * With a null ps mbrtowc() keeps its own state between calls.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtowc.009
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
	wchar_t whole = 0, wc = 77;
	size_t n, r1, r2;

	pc_begin("XSH.mbrtowc.009");
	use_utf8();
	memset(&st, 0, sizeof st);
	n = mbrtowc(&whole, "\xC3\xA9", 2, &st);
	pc_check(n == 2, "setup: mbrtowc() of a two-byte character returned %zu", n);
	pc_test();
	r1 = mbrtowc(&wc, "\xC3", 1, NULL);
	r2 = mbrtowc(&wc, "\xA9", 1, NULL);
	pc_check(r1 == (size_t)-2, "first byte: mbrtowc() returned %zu, expected (size_t)-2", r1);
	pc_check(r2 == 1, "last byte: mbrtowc() returned %zu, expected 1", r2);
	pc_check(wc == whole, "the code stored through the internal state differs from the code of one call");
	pc_pass();
}
