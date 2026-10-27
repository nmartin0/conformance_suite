/*
 * The reentrant lookup of a name with no entry returns 0 and a null result.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getgrnam.004
 */
#include "grdb.h"

int main(void)
{
	char name[64], buf[16384];
	xent_t e, other, *res = &other;
	int r;

	pc_begin("XSH.getgrnam.004");
	snprintf(name, sizeof name, "pcnoent%ldx", (long)getpid());
	memset(&e, 0, sizeof e);
	pc_test();
	r = getgrnam_r(name, &e, buf, sizeof buf, &res);
	if (r != 0 && xc_mayfail(r))
		pc_unresolved("the lookup reported %s", pc_errname(r));
	pc_check(r == 0, "returned %d (%s), not 0", r, pc_errname(r));
	pc_check(res == NULL, "*result is not null although no entry has the name");
	pc_pass();
}
