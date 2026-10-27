/*
 * In a UTF-8 locale mbrtowc() returns the length of one-, two- and
 * three-byte characters, and wcrtomb() turns the stored codes back into the
 * same bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtowc.005
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
	static const char *const s[3] = { "A", "\xC3\xA9", "\xE2\x82\xAC" };
	static const size_t want[3] = { 1, 2, 3 };
	mbstate_t st;
	wchar_t wc[3];
	char buf[16];
	size_t r[3], w[3];
	int i;

	pc_begin("XSH.mbrtowc.005");
	use_utf8();
	pc_test();
	for (i = 0; i < 3; i++) {
		memset(&st, 0, sizeof st);
		wc[i] = 0;
		r[i] = mbrtowc(&wc[i], s[i], 4, &st);
		pc_check(r[i] == want[i], "mbrtowc() of string %d returned %zu, expected %zu", i, r[i], want[i]);
	}
	pc_check(wc[0] == (wchar_t)'A', "mbrtowc() of \"A\" stored %ld", (long)wc[0]);
	for (i = 0; i < 3; i++) {
		memset(&st, 0, sizeof st);
		memset(buf, 'x', sizeof buf);
		w[i] = wcrtomb(buf, wc[i], &st);
		pc_check(w[i] == want[i] && memcmp(buf, s[i], want[i]) == 0, "wcrtomb() of the code stored for string %d returned %zu or other bytes", i, w[i]);
	}
	pc_pass();
}
