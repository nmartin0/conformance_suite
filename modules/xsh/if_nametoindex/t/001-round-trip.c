/*
 * if_nametoindex() of each name if_nameindex() lists returns that entry's index.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.if_nametoindex.001
 */
#include "if.h"

int main(void)
{
	struct if_nameindex *l;
	int i, n;

	pc_begin("XSH.if_nametoindex.001");
	l = get_list();
	n = list_len(l);
	if (n == 0)
		pc_notinuse("no network interface exists");
	pc_test();
	for (i = 0; i < n; i++) {
		unsigned r = if_nametoindex(l[i].if_name);

		pc_check(r == l[i].if_index, "if_nametoindex(\"%s\") returned %u, expected %u", l[i].if_name, r, l[i].if_index);
	}
	if_freenameindex(l);
	pc_pass();
}
