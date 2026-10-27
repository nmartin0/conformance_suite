/*
 * catclose() of an open catalog returns 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catclose.001
 */
#include "cg.h"

int main(void)
{
	nl_catd cd;
	int r;

	pc_begin("XSH.catclose.001");
	cg_make("k1.cat", "k1.msg", CG_SRC);
	cd = catopen("./k1.cat", 0);
	pc_check(cd != CG_BAD, "catopen of the test catalog failed: %s", pc_errname(errno));
	pc_test();
	errno = 0;
	r = catclose(cd);
	pc_check(r == 0, "catclose returned %d (errno %s), expected 0", r, pc_errname(errno));
	pc_pass();
}
