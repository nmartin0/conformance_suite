/*
 * A lookup by the ID of an existing entry finds an entry with that ID, and its name leads back to it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getgrgid.001
 */
#include "grdb.h"

int main(void)
{
	struct xc c, d;
	xent_t *p, *q;

	pc_begin("XSH.getgrgid.001");
	xc_find(&c);
	pc_test();
	p = getgrgid(c.id);
	pc_check(p != NULL, "no entry found for ID %ld", (long)c.id);
	pc_check(XID(p) == c.id, "entry found has ID %ld, not %ld", (long)XID(p), (long)c.id);
	xc_set(&d, p);
	q = getgrnam(d.name);
	pc_check(q != NULL, "no entry found for the name %s of the entry", d.name);
	pc_check(XID(q) == c.id, "the name %s leads to ID %ld, not %ld", d.name, (long)XID(q), (long)c.id);
	pc_pass();
}
