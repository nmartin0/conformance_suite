/*
 * With a buffer too small for the entry the reentrant lookup returns ERANGE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getgrgid.005
 */
#include "grdb.h"

int main(void)
{
	struct xc c;
	xent_t e, other, *res = &other;
	char buf[1];
	int r;

	pc_begin("XSH.getgrgid.005");
	xc_find(&c);
	memset(&e, 0, sizeof e);
	pc_test();
	r = getgrgid_r(c.id, &e, buf, 1, &res);
	pc_check(r == ERANGE, "returned %d (%s) for a 1 byte buffer, not ERANGE", r, pc_errname(r));
	pc_check(res == NULL, "*result is not null after ERANGE");
	res = &other;
	r = getgrgid_r(c.id, &e, buf, 0, &res);
	pc_check(r == ERANGE, "returned %d (%s) for a 0 byte buffer, not ERANGE", r, pc_errname(r));
	pc_check(res == NULL, "*result is not null after ERANGE (size 0)");
	pc_pass();
}
