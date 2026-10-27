/*
 * Escape sequences and a continuation line in the gencat source come back as the characters they stand for.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catgets.003
 */
#include "cg.h"

static void want(nl_catd cd, int id, const char *text)
{
	const char *m = catgets(cd, 1, id, "default message");

	pc_check(strcmp(m, text) == 0, "message %d is \"%s\", expected \"%s\"", id, m, text);
}

int main(void)
{
	nl_catd cd;

	pc_begin("XSH.catgets.003");
	cg_make("c3.cat", "c3.msg",
	    "$set 1 escapes\n"
	    "1 tab\\there\\nnl\\\\bs\\101\n"
	    "2 join \\\n"
	    "me\n"
	    "3 x\\qy\n");
	cd = catopen("./c3.cat", 0);
	pc_check(cd != CG_BAD, "catopen of the test catalog failed: %s", pc_errname(errno));
	pc_test();
	want(cd, 1, "tab\there\nnl\\bsA");
	want(cd, 2, "join me");
	want(cd, 3, "xqy");
	(void)catclose(cd);
	pc_pass();
}
