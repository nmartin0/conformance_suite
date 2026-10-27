/*
 * catgets() returns the stored text of a message, for messages in two sets.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catgets.001
 */
#include "cg.h"

static void want(nl_catd cd, int set, int id, const char *text)
{
	const char *m = catgets(cd, set, id, "default message");

	pc_check(m != NULL, "catgets(set %d, id %d) returned a null pointer", set, id);
	pc_check(strcmp(m, text) == 0, "set %d, message %d is \"%s\", expected \"%s\"", set, id, m, text);
}

int main(void)
{
	nl_catd cd;

	pc_begin("XSH.catgets.001");
	cg_make("c1.cat", "c1.msg", CG_SRC);
	cd = catopen("./c1.cat", 0);
	pc_check(cd != CG_BAD, "catopen of the test catalog failed: %s", pc_errname(errno));
	pc_test();
	want(cd, 1, 1, "Hello catalog");
	want(cd, 1, 2, "Second line");
	want(cd, 4, 7, "Seven");
	want(cd, 4, 20, "Twenty");
	want(cd, 1, 1, "Hello catalog");
	(void)catclose(cd);
	pc_pass();
}
