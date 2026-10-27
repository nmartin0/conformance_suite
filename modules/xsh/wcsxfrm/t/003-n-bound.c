/*
 * wcsxfrm() places no more than n codes and returns the full length.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsxfrm.003
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

#define DN 128

int main(void)
{
	wchar_t d[16], big[DN];
	size_t full, part;
	int i;

	pc_begin("XSH.wcsxfrm.003");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	for (i = 0; i < 16; i++)
		d[i] = L'X';
	pc_test();
	full = wcsxfrm(big, L"abcdef", DN);
	if (full >= DN)
		pc_unresolved("the transformed string needs %zu codes, more than the %d provided", full, DN);
	part = wcsxfrm(d, L"abcdef", 3);
	for (i = 3; i < 16; i++)
		pc_check(d[i] == L'X', "d[%d] was written with n 3", i);
	pc_check(part == full, "with n 3 the call returned %zu, with room for everything %zu", part, full);
	pc_pass();
}
