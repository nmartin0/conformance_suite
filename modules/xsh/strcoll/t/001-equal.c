/*
 * strcoll() of equal strings.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcoll.001
 */
#include "coll.h"

int main(void)
{
	char a1[] = "abc", a2[] = "abc", b1[] = "a b", b2[] = "a b", c1[] = "", c2[] = "";
	int r;

	pc_begin("XSH.strcoll.001");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	r = strcoll(a1, a2);
	pc_check(r == 0, "strcoll(\"abc\", \"abc\") returned %d", r);
	r = strcoll(b1, b2);
	pc_check(r == 0, "strcoll(\"a b\", \"a b\") returned %d", r);
	r = strcoll(c1, c2);
	pc_check(r == 0, "strcoll(\"\", \"\") returned %d", r);
	pc_pass();
}
