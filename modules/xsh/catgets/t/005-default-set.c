/*
 * Messages given without a $set directive are in the set NL_SETD.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catgets.005
 */
#include "cg.h"

int main(void)
{
	nl_catd cd;
	const char *m;

	pc_begin("XSH.catgets.005");
	/* gencat works here: it builds a catalog whose messages are in a set */
	cg_make("c5s.cat", "c5s.msg", CG_SRC);
	cd = catopen("./c5s.cat", 0);
	pc_check(cd != CG_BAD, "catopen of the test catalog failed: %s", pc_errname(errno));
	(void)catclose(cd);
	pc_make_file("c5.msg",
	    "1 default set one\n"
	    "2 default set two\n");
	pc_test();
	/* a source without $set is valid; its messages go to NL_SETD */
	pc_check(system("gencat ./c5.cat ./c5.msg >/dev/null 2>&1") == 0,
	    "gencat refused a message source without a $set directive");
	cd = catopen("./c5.cat", 0);
	pc_check(cd != CG_BAD, "catopen of the catalog without a set failed: %s", pc_errname(errno));
	m = catgets(cd, NL_SETD, 1, "default message");
	pc_check(strcmp(m, "default set one") == 0, "NL_SETD message 1 is \"%s\"", m);
	m = catgets(cd, NL_SETD, 2, "default message");
	pc_check(strcmp(m, "default set two") == 0, "NL_SETD message 2 is \"%s\"", m);
	(void)catclose(cd);
	pc_pass();
}
