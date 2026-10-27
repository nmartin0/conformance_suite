/*
 * strcasecmp: strcasecmp_l with a locale object.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcasecmp.004
 */
#include "sc.h"

int main(void)
{
	locale_t loc;

	pc_begin("XSH.strcasecmp.004");
	sc_init();
	loc = sc_cloc();
	pc_test();
	sc_expect(1, "abc", "ABC", 0, loc, 0);
	sc_expect(1, "", "", 0, loc, 0);
	sc_expect(1, "ab", "ABC", 0, loc, -1);
	sc_expect(1, "ABC", "ab", 0, loc, 1);
	sc_expect(1, "", "a", 0, loc, -1);
	sc_expect(1, "A", "", 0, loc, 1);
	sc_order(1, 0, loc);
	freelocale(loc);
	pc_pass();
}
