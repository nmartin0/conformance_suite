/*
 * if_indextoname() of each index if_nameindex() lists returns the buffer, holding that entry's name.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.if_indextoname.001
 */
#include "if.h"

int main(void)
{
	struct if_nameindex *l;
	int i, n;

	pc_begin("XSH.if_indextoname.001");
	l = get_list();
	n = list_len(l);
	if (n == 0)
		pc_notinuse("no network interface exists");
	pc_test();
	for (i = 0; i < n; i++) {
		char buf[IF_NAMESIZE];
		char *r;

		memset(buf, 0xA5, sizeof buf);
		r = if_indextoname(l[i].if_index, buf);
		pc_check(r == buf, "if_indextoname(%u) did not return the buffer", l[i].if_index);
		pc_check(memchr(buf, '\0', sizeof buf) != NULL && strcmp(buf, l[i].if_name) == 0,
		    "if_indextoname(%u) gave a name other than \"%s\"", l[i].if_index, l[i].if_name);
	}
	if_freenameindex(l);
	pc_pass();
}
