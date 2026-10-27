/*
 * Names read from the database are found again by a lookup by name.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getpwent.006
 */
#include "pwdb.h"

int main(void)
{
	struct xc c[16];
	xent_t *p, *q;
	int n = 0, i;

	pc_begin("XSH.getpwent.006");
	pc_test();
	setpwent();
	while (n < 16 && (p = getpwent()) != NULL)
		xc_set(&c[n++], p);
	endpwent();
	if (n == 0)
		pc_unresolved("no entry is visible");
	for (i = 0; i < n; i++) {
		q = getpwnam(c[i].name);
		pc_check(q != NULL, "no entry found for the name %s read from the database", c[i].name);
		pc_check(XNAME(q) != NULL && strcmp(XNAME(q), c[i].name) == 0, "the name %s found another entry", c[i].name);
	}
	pc_pass();
}
