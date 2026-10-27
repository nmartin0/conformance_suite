/*
 * No more than n bytes are written, and the return value does not depend on n.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strxfrm.003
 */
#include "lc.h"

int main(void)
{
	char full[256], small[32];
	size_t r, all, i;

	pc_begin("XSH.strxfrm.003");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	memset(small, 'X', sizeof small);
	pc_test();
	all = strxfrm(full, "abcdef", sizeof full);
	if (all >= sizeof full)
		pc_unresolved("the transformed string needs %zu bytes, more than the %zu provided", all, sizeof full);
	r = strxfrm(small, "abcdef", 3);
	pc_check(r == all, "with n 3 strxfrm() returned %zu, with room for everything %zu", r, all);
	for (i = 3; i < sizeof small; i++)
		pc_check(small[i] == 'X', "with n 3 the byte at index %zu was written", i);
	pc_pass();
}
