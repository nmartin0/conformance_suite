/*
 * In a UTF-8 locale mbrlen() reports the length of ASCII, two-byte and
 * three-byte characters.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrlen.003
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
	size_t r[3];
	int i;

	pc_begin("XSH.mbrlen.003");
	use_utf8();
	pc_test();
	for (i = 0; i < 3; i++) {
		memset(&st, 0, sizeof st);
		r[i] = mbrlen(s[i], 4, &st);
		pc_check(r[i] == want[i], "mbrlen() of string %d returned %zu, expected %zu", i, r[i], want[i]);
	}
	pc_pass();
}
