/*
 * mblen() returns -1 when the n bytes do not hold a complete character.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mblen.004
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
	int r2, r3, r4;

	pc_begin("XSH.mblen.004");
	use_utf8();
	pc_test();
	r2 = mblen("\xC3\xA9", 1);
	r3 = mblen("\xE2\x82\xAC", 2);
	r4 = mblen("\xC3\xA9", 2);
	pc_check(r2 == -1, "mblen() of a two-byte character with n 1 returned %d, expected -1", r2);
	pc_check(r3 == -1, "mblen() of a three-byte character with n 2 returned %d, expected -1", r3);
	pc_check(r4 == 2, "mblen() of a two-byte character with n 2 returned %d, expected 2", r4);
	pc_pass();
}
