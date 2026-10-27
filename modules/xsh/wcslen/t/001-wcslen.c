/*
 * wcslen() counts the codes before the null code.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcslen.001
 */
#include "pc.h"
#include <wchar.h>

static wchar_t empty[1], a[] = { L'a', 0 }, big[4096], mid[] = { L'a', L'b', 0, L'c' };

int main(void)
{
	size_t r[4];
	int i;

	pc_begin("XSH.wcslen.001");
	for (i = 0; i < 4095; i++)
		big[i] = L'b';
	pc_test();
	r[0] = wcslen(empty);
	r[1] = wcslen(a);
	r[2] = wcslen(big);
	r[3] = wcslen(mid);
	pc_check(r[0] == 0, "wcslen(\"\") returned %zu", r[0]);
	pc_check(r[1] == 1, "wcslen(\"a\") returned %zu", r[1]);
	pc_check(r[2] == 4095, "wcslen() of a 4095-code string returned %zu", r[2]);
	pc_check(r[3] == 2, "wcslen() of a string followed by more codes returned %zu", r[3]);
	pc_pass();
}
