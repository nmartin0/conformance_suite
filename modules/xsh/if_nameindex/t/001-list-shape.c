/*
 * if_nameindex() gives one entry per interface, with positive unique indexes and unique names, ended by an entry of index 0 and null name.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.if_nameindex.001
 */
#include "if.h"

int main(void)
{
	struct if_nameindex *l;
	int i, j, n;

	pc_begin("XSH.if_nameindex.001");
	pc_test();
	l = if_nameindex();
	if (l == NULL)
		pc_unresolved("if_nameindex() failed: %s", pc_errname(errno));
	for (n = 0; l[n].if_index != 0 || l[n].if_name != NULL; n++) {
		pc_check(l[n].if_index != 0, "entry %d has index 0 but a name", n);
		pc_check(l[n].if_name != NULL, "entry %d (index %u) has a null name", n, l[n].if_index);
		pc_check(memchr(l[n].if_name, '\0', IF_NAMESIZE) != NULL, "entry %d: the name is not within IF_NAMESIZE bytes", n);
		pc_check(l[n].if_name[0] != '\0', "entry %d: the name is empty", n);
		pc_check(n < 100000, "the list has no end");
	}
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(l[i].if_index != l[j].if_index, "entries %d and %d share index %u", i, j, l[i].if_index);
			pc_check(strcmp(l[i].if_name, l[j].if_name) != 0, "entries %d and %d share the name %s", i, j, l[i].if_name);
		}
	if (n == 0) {
		if_freenameindex(l);
		pc_notinuse("no network interface exists");
	}
	if_freenameindex(l);
	pc_pass();
}
