/*
 * wcsnlen() returns the smaller of the length and maxlen.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcslen.002
 */
#include "pc.h"
#include <wchar.h>

static wchar_t s[] = { L'a', L'b', L'c', L'd', L'e', L'f', 0, 0, 0, 0 };
static wchar_t empty[8], mid[] = { L'a', L'b', 0, L'c', L'd' };

int main(void)
{
	static const size_t m[4] = { 10, 6, 3, 0 }, want[4] = { 6, 6, 3, 0 };
	size_t r[6];
	int i;

	pc_begin("XSH.wcslen.002");
	pc_test();
	for (i = 0; i < 4; i++)
		r[i] = wcsnlen(s, m[i]);
	r[4] = wcsnlen(empty, 5);
	r[5] = wcsnlen(mid, 5);
	for (i = 0; i < 4; i++)
		pc_check(r[i] == want[i], "wcsnlen(\"abcdef\", %zu) returned %zu, expected %zu", m[i], r[i], want[i]);
	pc_check(r[4] == 0, "wcsnlen(\"\", 5) returned %zu", r[4]);
	pc_check(r[5] == 2, "wcsnlen() of {a, b, null, c, d} with maxlen 5 returned %zu, expected 2", r[5]);
	pc_pass();
}
