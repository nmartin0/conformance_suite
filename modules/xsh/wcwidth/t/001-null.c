/*
 * wcwidth() of the null code is 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcwidth.001
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

int main(void)
{
	int r;

	pc_begin("XSH.wcwidth.001");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	pc_test();
	r = wcwidth(L'\0');
	pc_check(r == 0, "wcwidth(0) returned %d", r);
	pc_pass();
}
