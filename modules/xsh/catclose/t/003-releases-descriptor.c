/*
 * Closing a catalog releases any file descriptor used for it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catclose.003
 */
#include "cg.h"

int main(void)
{
	static unsigned char before[CG_FDS], after[CG_FDS];
	nl_catd cd;
	int fd;

	pc_begin("XSH.catclose.003");
	cg_make("k3.cat", "k3.msg", CG_SRC);
	/* a first open and close, so that anything the implementation keeps open for good is already open */
	cd = catopen("./k3.cat", 0);
	if (cd == CG_BAD)
		pc_unresolved("catopen of the test catalog failed: %s", pc_errname(errno));
	(void)catclose(cd);
	pc_test();
	cg_fdmap(before);
	cd = catopen("./k3.cat", 0);
	pc_check(cd != CG_BAD, "catopen failed: %s", pc_errname(errno));
	pc_check(catclose(cd) == 0, "catclose failed");
	cg_fdmap(after);
	for (fd = 0; fd < CG_FDS; fd++)
		pc_check(before[fd] == after[fd], "descriptor %d is %s after catopen and catclose",
		    fd, after[fd] ? "open although it was closed before" : "closed although it was open before");
	pc_pass();
}
