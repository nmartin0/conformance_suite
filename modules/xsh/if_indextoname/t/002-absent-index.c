/*
 * if_indextoname() of an index that no interface has returns a null pointer and sets ENXIO.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.if_indextoname.002
 */
#include "if.h"

static int has_index(const struct if_nameindex *l, unsigned v)
{
	int i;

	for (i = 0; l[i].if_index != 0 || l[i].if_name != NULL; i++)
		if (l[i].if_index == v)
			return 1;
	return 0;
}

int main(void)
{
	struct if_nameindex *l;
	unsigned max = 0, cand[3];
	int i, n;

	pc_begin("XSH.if_indextoname.002");
	l = get_list();
	n = list_len(l);
	for (i = 0; i < n; i++)
		if (l[i].if_index > max)
			max = l[i].if_index;
	cand[0] = 0;		/* indexes start at 1 */
	cand[1] = max + 1;
	cand[2] = UINT_MAX;
	pc_test();
	for (i = 0; i < 3; i++) {
		char buf[IF_NAMESIZE];
		char *r;

		if (has_index(l, cand[i]))
			continue;
		errno = 0;
		r = if_indextoname(cand[i], buf);
		pc_check(r == NULL && errno == ENXIO, "if_indextoname(%u) returned %s, errno %s; expected NULL and ENXIO", cand[i], r ? "a pointer" : "NULL", pc_errname(errno));
	}
	if_freenameindex(l);
	pc_pass();
}
