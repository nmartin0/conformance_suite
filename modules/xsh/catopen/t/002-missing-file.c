/*
 * A pathname naming no catalog fails with ENOENT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catopen.002
 */
#include "cg.h"

int main(void)
{
	nl_catd cd;
	const char *m;

	pc_begin("XSH.catopen.002");
	pc_test();
	errno = 0;
	cd = catopen("./pc-no-dir/none.cat", 0);
	if (cd == CG_BAD) {
		pc_check(errno == ENOENT, "errno %s, expected ENOENT", pc_errname(errno));
		pc_pass();
	}
	/* the page lists the error as one that may occur: a descriptor is tolerated, but it must be harmless */
	m = catgets(cd, 1, 1, "default");
	pc_check(strcmp(m, "default") == 0, "catgets on a catalog that does not exist gave \"%s\"", m);
	pc_check(catclose(cd) == 0, "catclose failed");
	pc_pass();
}
