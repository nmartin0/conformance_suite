/*
 * wcstok() overwrites only the separator that ends each token.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstok.002
 */
#include "pc.h"
#include <wchar.h>

static wchar_t s[] = L"  ab,,cd e  ", sep[] = L" ,";
static const wchar_t want[13] = { L' ', L' ', L'a', L'b', 0, L',', L'c', L'd', 0, L'e', 0, L' ', 0 };

int main(void)
{
	wchar_t *save = NULL;
	int i;

	pc_begin("XSH.wcstok.002");
	pc_test();
	(void)wcstok(s, sep, &save);
	for (i = 0; i < 4; i++)
		(void)wcstok(NULL, sep, &save);
	for (i = 0; i < 13; i++)
		pc_check(s[i] == want[i], "after the sequence s[%d] is %#lx, expected %#lx",
		    i, (unsigned long)s[i], (unsigned long)want[i]);
	pc_pass();
}
