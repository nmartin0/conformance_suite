/*
 * An empty name fails with ENOENT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catopen.004
 */
#include "cg.h"

int main(void)
{
	nl_catd cd;
	const char *m;

	pc_begin("XSH.catopen.004");
	cg_nlspath_guard();
	pc_test();
	errno = 0;
	cd = catopen("", 0);
	if (cd == CG_BAD) {
		pc_check(errno == ENOENT, "errno %s, expected ENOENT", pc_errname(errno));
		pc_pass();
	}
	m = catgets(cd, 1, 1, "default");
	pc_check(strcmp(m, "default") == 0, "catgets gave \"%s\"", m);
	pc_check(catclose(cd) == 0, "catclose failed");
	pc_pass();
}
