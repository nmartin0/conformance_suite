/*
 * Reading the whole database visits the entry that a lookup finds for the caller.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getpwent.002
 */
#include "pwdb.h"

int main(void)
{
	struct xc c;
	xent_t *p;
	long n = 0;
	int found = 0;

	pc_begin("XSH.getpwent.002");
	xc_find(&c);
	if (c.id != getuid() && c.id != geteuid())
		pc_unresolved("the caller has no entry; the one found belongs to another user, which the system may hide");
	pc_test();
	setpwent();
	while ((p = getpwent()) != NULL) {
		if (++n > 1000000)
			pc_unresolved("more than a million entries");
		if (XNAME(p) != NULL && strcmp(XNAME(p), c.name) == 0 && XID(p) == c.id)
			found = 1;
	}
	endpwent();
	pc_check(found, "the entry %s (ID %ld) was not visited in %ld entries", c.name, (long)c.id, n);
	pc_pass();
}
