/*
 * After the end function the next read opens the database again at the first entry.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getpwent.004
 */
#include "pwdb.h"

int main(void)
{
	struct xc f, g;
	xent_t *p;

	pc_begin("XSH.getpwent.004");
	pc_test();
	setpwent();
	p = getpwent();
	if (p == NULL)
		pc_unresolved("no entry is visible");
	xc_set(&f, p);
	(void)getpwent();
	endpwent();
	p = getpwent();
	pc_check(p != NULL, "no entry after the end function");
	xc_set(&g, p);
	pc_check(xc_same(&f, &g), "after the end function the first entry is %s, not %s", g.name, f.name);
	(void)getpwent();
	endpwent();
	setpwent();
	p = getpwent();
	pc_check(p != NULL, "no entry after the end and set functions");
	xc_set(&g, p);
	pc_check(xc_same(&f, &g), "after the end and set functions the first entry is %s, not %s", g.name, f.name);
	endpwent();
	pc_pass();
}
