/*
 * strncasecmp: strncasecmp_l with a locale object.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncasecmp.004
 */
#include "sc.h"

int main(void)
{
	locale_t loc;

	pc_begin("XSH.strncasecmp.004");
	sc_init();
	loc = sc_cloc();
	pc_test();
	sc_expect(3, "abc", "ABC", 3, loc, 0);
	sc_expect(3, "a", "B", 1, loc, -1);
	sc_expect(3, "abcd", "ABXY", 2, loc, 0);
	sc_expect(3, "abcd", "ABXY", 3, loc, SC_SGN('c', 'x'));
	sc_expect(3, "abcd", "ABXY", 0, loc, 0);
	sc_expect(3, "ab", "ABC", 5, loc, -1);
	sc_order(3, 40, loc);
	freelocale(loc);
	pc_pass();
}
