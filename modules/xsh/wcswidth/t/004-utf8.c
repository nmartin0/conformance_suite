/*
 * wcswidth() of portable characters in a UTF-8 locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcswidth.004
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

int main(void)
{
	const char *loc;
	int r1, r2;

	pc_begin("XSH.wcswidth.004");
	loc = pc_find_locale("UTF-8", 1);
	if (loc == NULL)
		pc_notinuse("no UTF-8 locale installed");
	pc_check(setlocale(LC_ALL, loc) != NULL, "setlocale(LC_ALL, \"%s\") failed", loc);
	pc_test();
	r1 = wcswidth(L"abc", 3);
	r2 = wcswidth(L"", 5);
	pc_check(r1 == 3, "wcswidth(\"abc\", 3) returned %d", r1);
	pc_check(r2 == 0, "wcswidth(\"\", 5) returned %d", r2);
	pc_pass();
}
