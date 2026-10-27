/*
 * [XSI] NLSPATH: when the first template finds nothing, the next one is used.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catopen.007
 */
#include "cg.h"

int main(void)
{
	nl_catd cd;
	const char *m;

	pc_begin("XSH.catopen.007");
	cg_nlspath_guard();
	cg_make("c007x", "c007.msg", CG_SRC);
	cg_setpath("./pc-no-dir/%N:./%N");
	pc_test();
	errno = 0;
	cd = catopen("c007x", 0);
	pc_check(cd != CG_BAD, "catopen did not try the second template: %s", pc_errname(errno));
	m = catgets(cd, 4, 7, "default");
	pc_check(strcmp(m, "Seven") == 0, "message is \"%s\"", m);
	pc_check(catclose(cd) == 0, "catclose failed");
	pc_pass();
}
