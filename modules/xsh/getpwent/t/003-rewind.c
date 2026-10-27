/*
 * The set function rewinds so that the next read returns the first entry again.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getpwent.003
 */
#include "pwdb.h"

int main(void)
{
	struct xc f, g;
	xent_t *p;
	long n = 0;

	pc_begin("XSH.getpwent.003");
	pc_test();
	setpwent();
	p = getpwent();
	if (p == NULL)
		pc_unresolved("no entry is visible");
	xc_set(&f, p);
	(void)getpwent();
	setpwent();
	p = getpwent();
	pc_check(p != NULL, "no entry after rewinding in the middle");
	xc_set(&g, p);
	pc_check(xc_same(&f, &g), "after rewinding in the middle the first entry is %s, not %s", g.name, f.name);
	while (getpwent() != NULL)
		if (++n > 1000000)
			pc_unresolved("more than a million entries");
	setpwent();
	p = getpwent();
	pc_check(p != NULL, "no entry after rewinding at the end");
	xc_set(&g, p);
	pc_check(xc_same(&f, &g), "after rewinding at the end the first entry is %s, not %s", g.name, f.name);
	endpwent();
	pc_pass();
}
