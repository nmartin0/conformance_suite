/*
 * The reentrant lookup of an ID with no entry returns 0 and a null result.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getpwuid.004
 */
#include "pwdb.h"

int main(void)
{
	char buf[16384];
	xent_t e, other, *res = &other;
	xid_t id;
	int r;

	pc_begin("XSH.getpwuid.004");
	id = xc_absent_id();
	memset(&e, 0, sizeof e);
	pc_test();
	r = getpwuid_r(id, &e, buf, sizeof buf, &res);
	if (r != 0 && xc_mayfail(r))
		pc_unresolved("the lookup reported %s", pc_errname(r));
	pc_check(r == 0, "returned %d (%s), not 0", r, pc_errname(r));
	pc_check(res == NULL, "*result is not null although no entry has the ID");
	pc_pass();
}
