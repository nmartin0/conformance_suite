/*
 * The set function rewinds so that the next read returns the first entry again.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getgrent.003
 */
#include "grdb.h"

int main(void)
{
	struct xc f, g;
	xent_t *p;
	long n = 0;

	pc_begin("XSH.getgrent.003");
	pc_test();
	setgrent();
	p = getgrent();
	if (p == NULL)
		pc_unresolved("no entry is visible");
	xc_set(&f, p);
	(void)getgrent();
	setgrent();
	p = getgrent();
	pc_check(p != NULL, "no entry after rewinding in the middle");
	xc_set(&g, p);
	pc_check(xc_same(&f, &g), "after rewinding in the middle the first entry is %s, not %s", g.name, f.name);
	while (getgrent() != NULL)
		if (++n > 1000000)
			pc_unresolved("more than a million entries");
	setgrent();
	p = getgrent();
	pc_check(p != NULL, "no entry after rewinding at the end");
	xc_set(&g, p);
	pc_check(xc_same(&f, &g), "after rewinding at the end the first entry is %s, not %s", g.name, f.name);
	endgrent();
	pc_pass();
}
