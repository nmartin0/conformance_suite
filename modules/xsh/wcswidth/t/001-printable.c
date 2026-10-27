/*
 * wcswidth() counts the columns of printable portable characters.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcswidth.001
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

int main(void)
{
	int r[5];

	pc_begin("XSH.wcswidth.001");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	pc_test();
	r[0] = wcswidth(L"abc", 3);
	r[1] = wcswidth(L"abc", 2);
	r[2] = wcswidth(L"abc", 10);
	r[3] = wcswidth(L"a b", 3);
	r[4] = wcswidth(L"0!~", 3);
	pc_check(r[0] == 3, "wcswidth(\"abc\", 3) returned %d", r[0]);
	pc_check(r[1] == 2, "wcswidth(\"abc\", 2) returned %d", r[1]);
	pc_check(r[2] == 3, "wcswidth(\"abc\", 10) returned %d", r[2]);
	pc_check(r[3] == 3, "wcswidth(\"a b\", 3) returned %d", r[3]);
	pc_check(r[4] == 3, "wcswidth(\"0!~\", 3) returned %d", r[4]);
	pc_pass();
}
