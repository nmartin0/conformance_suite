/*
 * wcswidth() of nothing is 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcswidth.002
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

int main(void)
{
	int r1, r2;

	pc_begin("XSH.wcswidth.002");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	pc_test();
	r1 = wcswidth(L"", 5);
	r2 = wcswidth(L"abc", 0);
	pc_check(r1 == 0, "wcswidth(\"\", 5) returned %d", r1);
	pc_check(r2 == 0, "wcswidth(\"abc\", 0) returned %d", r2);
	pc_pass();
}
