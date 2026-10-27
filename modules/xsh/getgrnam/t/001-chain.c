/*
 * A lookup by the name of an existing entry finds that entry.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getgrnam.001
 */
#include "grdb.h"

int main(void)
{
	struct xc c, d;
	xent_t *p;

	pc_begin("XSH.getgrnam.001");
	xc_find(&c);
	pc_test();
	p = getgrnam(c.name);
	pc_check(p != NULL, "no entry found for the name %s of an entry", c.name);
	pc_check(XNAME(p) != NULL && strcmp(XNAME(p), c.name) == 0, "entry found has another name");
	xc_set(&d, p);
	pc_check(d.id == c.id, "ID %ld differs from %ld", (long)d.id, (long)c.id);
	pc_pass();
}
