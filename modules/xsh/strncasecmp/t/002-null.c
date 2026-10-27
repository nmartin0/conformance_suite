/*
 * strncasecmp: the null byte ends the comparison.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncasecmp.002
 */
#include "sc.h"

int main(void)
{
	pc_begin("XSH.strncasecmp.002");
	sc_init();
	pc_test();
	static const char a[4] = { 'a', 'b', 0, 'x' }, b[4] = { 'A', 'B', 0, 'y' };

	pc_check(sc_cmp(2, a, b, 4, (locale_t)0) == 0, "arrays equal up to a null byte did not compare equal for n 4");
	pc_check(sc_cmp(2, b, a, 4, (locale_t)0) == 0, "arrays equal up to a null byte did not compare equal for n 4 (swapped)");
	sc_expect(2, "ab", "ABC", 5, (locale_t)0, -1);
	sc_expect(2, "ABC", "ab", 5, (locale_t)0, 1);
	sc_expect(2, "abc", "ABC", 100, (locale_t)0, 0);
	sc_expect(2, "", "", 100, (locale_t)0, 0);
	sc_expect(2, "", "a", 100, (locale_t)0, -1);
	sc_expect(2, "A", "", 100, (locale_t)0, 1);
	pc_pass();
}
