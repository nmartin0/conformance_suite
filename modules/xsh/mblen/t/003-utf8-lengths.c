/*
 * In a UTF-8 locale mblen() reports the length of ASCII, two-byte and
 * three-byte characters, 0 for the null byte, and 0 for a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mblen.003
 */
#include "pc.h"
#include <locale.h>
#include <stdlib.h>

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
	static const char *const s[4] = { "A", "\xC3\xA9", "\xE2\x82\xAC", "" };
	static const int want[4] = { 1, 2, 3, 0 };
	int r[4], rn, i;

	pc_begin("XSH.mblen.003");
	use_utf8();
	pc_test();
	for (i = 0; i < 4; i++)
		r[i] = mblen(s[i], 4);
	rn = mblen(NULL, 0);
	for (i = 0; i < 4; i++) {
		pc_check(r[i] == want[i], "mblen() of string %d returned %d, expected %d", i, r[i], want[i]);
		pc_check(r[i] <= 4 && r[i] <= (int)MB_CUR_MAX, "mblen() returned %d, more than n or MB_CUR_MAX", r[i]);
	}
	pc_check(rn == 0, "mblen(NULL, 0) returned %d, expected 0", rn);
	pc_pass();
}
