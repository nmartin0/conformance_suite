/*
 * wcwidth() of a printable portable character is 1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcwidth.002
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

/* the 95 printable characters of the portable character set */
static const wchar_t pr[] = L" !\"#$%&'()*+,-./0123456789:;<=>?@"
    L"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

int main(void)
{
	int i, r;

	pc_begin("XSH.wcwidth.002");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	pc_check(sizeof pr / sizeof pr[0] == 96, "the table of printable characters has the wrong size");
	pc_test();
	for (i = 0; pr[i] != 0; i++) {
		r = wcwidth(pr[i]);
		pc_check(r == 1, "wcwidth() of printable character %#x (index %d) returned %d", (unsigned)pr[i], i, r);
	}
	pc_pass();
}
