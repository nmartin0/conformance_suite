/*
 * A message not in the catalog gives the default string and ENOMSG.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catgets.002
 */
#include "cg.h"

static void missing(nl_catd cd, int set, int id)
{
	static char dflt[] = "the default";
	char *m;

	errno = 0;
	m = catgets(cd, set, id, dflt);
	pc_check(m == dflt, "catgets(set %d, id %d) did not return the default string", set, id);
	pc_check(errno == ENOMSG, "catgets(set %d, id %d): errno %s, expected ENOMSG", set, id, pc_errname(errno));
}

int main(void)
{
	nl_catd cd;

	pc_begin("XSH.catgets.002");
	cg_make("c2.cat", "c2.msg", CG_SRC);
	cd = catopen("./c2.cat", 0);
	pc_check(cd != CG_BAD, "catopen of the test catalog failed: %s", pc_errname(errno));
	pc_test();
	missing(cd, 1, 3);
	missing(cd, 4, 8);
	missing(cd, 9, 1);
	missing(cd, 1, 7);
	(void)catclose(cd);
	pc_pass();
}
