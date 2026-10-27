/*
 * With n 0 the destination may be a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strxfrm.004
 */
#include "lc.h"

int main(void)
{
	char full[256], guard[8];
	size_t r, all, i;

	pc_begin("XSH.strxfrm.004");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	memset(guard, 'X', sizeof guard);
	pc_test();
	all = strxfrm(full, "abc", sizeof full);
	if (all >= sizeof full)
		pc_unresolved("the transformed string needs %zu bytes, more than the %zu provided", all, sizeof full);
	r = strxfrm(NULL, "abc", 0);
	pc_check(r == all, "strxfrm(NULL, s, 0) returned %zu, with room for everything %zu", r, all);
	r = strxfrm(guard, "abc", 0);
	pc_check(r == all, "strxfrm(buf, s, 0) returned %zu, with room for everything %zu", r, all);
	for (i = 0; i < sizeof guard; i++)
		pc_check(guard[i] == 'X', "with n 0 the byte at index %zu was written", i);
	pc_pass();
}
