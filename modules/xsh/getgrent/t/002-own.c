/*
 * Reading the whole database visits the entry that a lookup finds for the caller.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getgrent.002
 */
#include "grdb.h"

int main(void)
{
	struct xc c;
	xent_t *p;
	long n = 0;
	int found = 0;

	pc_begin("XSH.getgrent.002");
	xc_find(&c);
	if (c.id != getgid() && c.id != getegid())
		pc_unresolved("the caller has no group entry; the one found is not associated with the caller, so the system may hide it");
	pc_test();
	setgrent();
	while ((p = getgrent()) != NULL) {
		if (++n > 1000000)
			pc_unresolved("more than a million entries");
		if (XNAME(p) != NULL && strcmp(XNAME(p), c.name) == 0 && XID(p) == c.id)
			found = 1;
	}
	endgrent();
	pc_check(found, "the entry %s (ID %ld) was not visited in %ld entries", c.name, (long)c.id, n);
	pc_pass();
}
