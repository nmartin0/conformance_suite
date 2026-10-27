/*
 * wcscoll() in the POSIX locale follows the listed collation sequence.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcscoll.002
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

/* first sorts before second in the POSIX-locale collation sequence */
static const struct { const wchar_t *first, *second; } pair[] = {
	{ L" ", L"0" }, { L"0", L"9" }, { L"9", L"A" }, { L"A", L"Z" },
	{ L"Z", L"a" }, { L"a", L"z" }, { L"ab", L"abc" }, { L"az", L"b" },
};
#define NPAIR (sizeof pair / sizeof pair[0])

int main(void)
{
	size_t i;
	int f, b;

	pc_begin("XSH.wcscoll.002");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	pc_test();
	for (i = 0; i < NPAIR; i++) {
		f = wcscoll(pair[i].first, pair[i].second);
		b = wcscoll(pair[i].second, pair[i].first);
		pc_check(f < 0, "pair %zu: wcscoll(first, second) returned %d, expected negative", i, f);
		pc_check(b > 0, "pair %zu: wcscoll(second, first) returned %d, expected positive", i, b);
	}
	pc_pass();
}
