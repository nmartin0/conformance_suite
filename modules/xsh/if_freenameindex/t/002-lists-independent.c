/*
 * Freeing one list from if_nameindex() leaves another, earlier or later, list usable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.if_freenameindex.002
 */
#include "if.h"

int main(void)
{
	struct if_nameindex *a, *b;
	char first[IF_NAMESIZE];
	unsigned idx;

	pc_begin("XSH.if_freenameindex.002");
	a = get_list();
	b = get_list();
	if (list_len(b) == 0)
		pc_notinuse("no network interface exists");
	pc_check(b[0].if_name != NULL && strlen(b[0].if_name) < IF_NAMESIZE, "the second list is malformed");
	strcpy(first, b[0].if_name);
	idx = b[0].if_index;
	pc_test();
	if_freenameindex(a);
	pc_check(b[0].if_index == idx && b[0].if_name != NULL && strcmp(b[0].if_name, first) == 0,
	    "the second list changed after the first was freed");
	if_freenameindex(b);
	pc_pass();
}
