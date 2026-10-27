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
	cg_make("c5.cat", "c5.msg",
	    "1 default set one\n"
	    "2 default set two\n");
	cd = catopen("./c5.cat", 0);
	pc_check(cd != CG_BAD, "catopen of the test catalog failed: %s", pc_errname(errno));
	pc_test();
	m = catgets(cd, NL_SETD, 1, "default message");
	pc_check(strcmp(m, "default set one") == 0, "NL_SETD message 1 is \"%s\"", m);
	m = catgets(cd, NL_SETD, 2, "default message");
	pc_check(strcmp(m, "default set two") == 0, "NL_SETD message 2 is \"%s\"", m);
	(void)catclose(cd);
	pc_pass();
}
