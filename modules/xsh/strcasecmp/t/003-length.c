/*
 * strcasecmp: end of string and symmetry.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcasecmp.003
 */
#include "sc.h"

int main(void)
{
	pc_begin("XSH.strcasecmp.003");
	sc_init();
	pc_test();
	static const char *const s[8] = { "", "a", "ab", "ABC", "abcd", "b", "A1", "_x" };
	int i, j, r1, r2;

	sc_expect(0, "ab", "ABC", 0, (locale_t)0, -1);
	sc_expect(0, "ABC", "ab", 0, (locale_t)0, 1);
	sc_expect(0, "", "a", 0, (locale_t)0, -1);
	sc_expect(0, "A", "", 0, (locale_t)0, 1);
	sc_expect(0, "abc", "ABCD", 0, (locale_t)0, -1);
	sc_expect(0, "abcd", "ABC", 0, (locale_t)0, 1);
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++) {
			r1 = sc_cmp(0, s[i], s[j], 0, (locale_t)0);
			r2 = sc_cmp(0, s[j], s[i], 0, (locale_t)0);
			pc_check(SC_SIGN(r1) == -SC_SIGN(r2), "\"%s\" and \"%s\": %d one way, %d the other", s[i], s[j], r1, r2);
		}
	pc_pass();
}
