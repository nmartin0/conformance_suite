/*
 * strncasecmp: at most n bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncasecmp.001
 */
#include "sc.h"

int main(void)
{
	pc_begin("XSH.strncasecmp.001");
	sc_init();
	pc_test();
	static const char x[3] = { 'a', 'b', 'c' }, y[3] = { 'A', 'B', 'C' };
	static const char p[2] = { 'a', 'b' }, q[2] = { 'A', 'B' };

	sc_expect(2, "abcd", "ABXY", 2, (locale_t)0, 0);
	sc_expect(2, "abcd", "ABXY", 3, (locale_t)0, SC_SGN('c', 'x'));
	sc_expect(2, "abcd", "ABXY", 4, (locale_t)0, SC_SGN('c', 'x'));
	sc_expect(2, "ABXY", "abcd", 3, (locale_t)0, SC_SGN('x', 'c'));
	sc_expect(2, "abcd", "ABXY", 0, (locale_t)0, 0);
	sc_expect(2, "abc", "xyz", 0, (locale_t)0, 0);
	sc_expect(2, "a", "B", 1, (locale_t)0, -1);
	sc_expect(2, "B", "a", 1, (locale_t)0, 1);
	sc_expect(2, "abc", "ABC", 3, (locale_t)0, 0);
	pc_check(sc_cmp(2, x, y, 3, (locale_t)0) == 0, "unterminated arrays of 3 bytes, equal ignoring case, did not compare equal");
	pc_check(sc_cmp(2, p, q, 2, (locale_t)0) == 0, "unterminated arrays of 2 bytes, equal ignoring case, did not compare equal");
	pc_pass();
}
