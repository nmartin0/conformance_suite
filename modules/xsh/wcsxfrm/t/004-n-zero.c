/*
 * wcsxfrm() with n 0 and a null pointer returns the length.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsxfrm.004
 */
#include "pc.h"
#include <locale.h>
#include <wchar.h>

#define DN 128

int main(void)
{
	wchar_t big[DN];
	size_t full, zero;

	pc_begin("XSH.wcsxfrm.004");
	pc_check(setlocale(LC_ALL, "C") != NULL, "setlocale(LC_ALL, \"C\") failed");
	pc_test();
	full = wcsxfrm(big, L"abcdef", DN);
	if (full >= DN)
		pc_unresolved("the transformed string needs %zu codes, more than the %d provided", full, DN);
	zero = wcsxfrm(NULL, L"abcdef", 0);
	pc_check(zero == full, "with n 0 the call returned %zu, with room for everything %zu", zero, full);
	pc_pass();
}
