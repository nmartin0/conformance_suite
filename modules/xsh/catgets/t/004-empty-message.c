/*
 * A message stored as empty text is returned as an empty string, not as the default.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catgets.004
 */
#include "cg.h"

int main(void)
{
	static char dflt[] = "the default";
	nl_catd cd;
	char *m;

	pc_begin("XSH.catgets.004");
	cg_make("c4.cat", "c4.msg",
	    "$set 1 empty\n"
	    "1 filled\n"
	    "5 \n"
	    "6 after\n");
	cd = catopen("./c4.cat", 0);
	pc_check(cd != CG_BAD, "catopen of the test catalog failed: %s", pc_errname(errno));
	pc_test();
	m = catgets(cd, 1, 5, dflt);
	pc_check(m != dflt, "an empty message gave the default string");
	pc_check(m != NULL && m[0] == '\0', "an empty message is \"%s\"", m);
	m = catgets(cd, 1, 6, dflt);
	pc_check(strcmp(m, "after") == 0, "the message after the empty one is \"%s\"", m);
	(void)catclose(cd);
	pc_pass();
}
