/*
 * wcwidth() of portable characters in a UTF-8 locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcwidth.004
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

static const wchar_t pr[] = L" !\"#$%&'()*+,-./0123456789:;<=>?@"
    L"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

int main(void)
{
	const char *loc;
	int i, r;

	pc_begin("XSH.wcwidth.004");
	loc = pc_find_locale("UTF-8", 1);
	if (loc == NULL)
		pc_notinuse("no UTF-8 locale installed");
	pc_check(setlocale(LC_ALL, loc) != NULL, "setlocale(LC_ALL, \"%s\") failed", loc);
	pc_test();
	r = wcwidth(L'\0');
	pc_check(r == 0, "wcwidth(0) returned %d", r);
	for (i = 0; pr[i] != 0; i++) {
		r = wcwidth(pr[i]);
		pc_check(r == 1, "wcwidth() of printable character %#x (index %d) returned %d", (unsigned)pr[i], i, r);
	}
	pc_pass();
}
