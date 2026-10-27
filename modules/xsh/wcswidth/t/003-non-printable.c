/*
 * wcswidth() is -1 when a non-printable code is among the first n.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcswidth.003
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

static wchar_t after_null[] = { L'a', L'b', 0, L'\t' };

int main(void)
{
	int r[5];

	pc_begin("XSH.wcswidth.003");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	pc_test();
	r[0] = wcswidth(L"a\tb", 3);
	r[1] = wcswidth(L"\n", 1);
	r[2] = wcswidth(L"a\tb", 1);
	r[3] = wcswidth(after_null, 4);
	r[4] = wcswidth(L"ab\t", 2);
	pc_check(r[0] == -1, "a tab among the first 3 codes: returned %d", r[0]);
	pc_check(r[1] == -1, "a newline as the first code: returned %d", r[1]);
	pc_check(r[2] == 1, "a tab beyond n changed the result (returned %d)", r[2]);
	pc_check(r[3] == 2, "a tab after the null code changed the result (returned %d)", r[3]);
	pc_check(r[4] == 2, "a tab beyond n=2 changed the result (returned %d)", r[4]);
	pc_pass();
}
