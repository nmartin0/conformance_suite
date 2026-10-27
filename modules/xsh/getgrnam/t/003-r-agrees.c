/*
 * The reentrant lookup by name returns the entry the plain lookup returns.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getgrnam.003
 */
#include "grdb.h"

int main(void)
{
	struct xc c, d, f;
	xent_t e, other, *res = &other, *p;
	char buf[16384];
	int r;

	pc_begin("XSH.getgrnam.003");
	xc_find(&c);
	p = getgrnam(c.name);
	pc_check(p != NULL, "no entry found for %s", c.name);
	xc_set(&d, p);
	memset(&e, 0, sizeof e);
	pc_test();
	r = getgrnam_r(c.name, &e, buf, sizeof buf, &res);
	if (r == ERANGE)
		pc_unresolved("16 KiB of buffer were not enough");
	pc_check(r == 0, "returned %d (%s)", r, pc_errname(r));
	pc_check(res == &e, "*result is not the structure passed");
	xc_set(&f, &e);
	pc_check(xc_same(&f, &d), "the entry differs from the one the plain lookup gives");
	pc_check(xc_inbuf(&e, buf, sizeof buf), "the entry refers to storage outside the buffer");
	pc_pass();
}
