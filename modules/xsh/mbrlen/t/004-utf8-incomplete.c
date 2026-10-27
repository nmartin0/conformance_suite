/*
 * mbrlen() returns (size_t)-2 for a character cut short by n, and the
 * state carries it: the call that supplies the rest returns the number of
 * bytes it supplied.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrlen.004
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
	size_t a1, a2, b1, b2, b3, c1, c2;

	pc_begin("XSH.mbrlen.004");
	use_utf8();
	pc_test();
	memset(&st, 0, sizeof st);
	a1 = mbrlen("\xC3", 1, &st);
	a2 = mbrlen("\xA9", 1, &st);
	pc_check(a1 == (size_t)-2, "mbrlen() of the first byte of a two-byte character returned %zu, expected (size_t)-2", a1);
	pc_check(a2 == 1, "mbrlen() of its last byte returned %zu, expected 1", a2);
	memset(&st, 0, sizeof st);
	b1 = mbrlen("\xE2", 1, &st);
	b2 = mbrlen("\x82", 1, &st);
	b3 = mbrlen("\xAC", 1, &st);
	pc_check(b1 == (size_t)-2 && b2 == (size_t)-2, "mbrlen() of the first two bytes of a three-byte character returned %zu and %zu, expected (size_t)-2", b1, b2);
	pc_check(b3 == 1, "mbrlen() of its last byte returned %zu, expected 1", b3);
	memset(&st, 0, sizeof st);
	c1 = mbrlen("\xE2\x82", 2, &st);
	c2 = mbrlen("\xAC", 1, &st);
	pc_check(c1 == (size_t)-2, "mbrlen() of two of three bytes returned %zu, expected (size_t)-2", c1);
	pc_check(c2 == 1, "mbrlen() of the last byte returned %zu, expected 1", c2);
	pc_pass();
}
