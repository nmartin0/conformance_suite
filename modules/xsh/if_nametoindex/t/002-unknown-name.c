/*
 * if_nametoindex() of a name that no interface has returns 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.if_nametoindex.002
 */
#include "if.h"

int main(void)
{
	static const char *const names[] = { "pc-no-such-if-zz", "", "0", "no such interface" };
	struct if_nameindex *l;
	size_t i;

	pc_begin("XSH.if_nametoindex.002");
	l = get_list();
	pc_test();
	for (i = 0; i < sizeof names / sizeof names[0]; i++) {
		unsigned r;

		if (in_list(l, names[i]))
			continue;
		r = if_nametoindex(names[i]);
		pc_check(r == 0, "if_nametoindex(\"%s\") returned %u, expected 0", names[i], r);
	}
	if_freenameindex(l);
	pc_pass();
}
