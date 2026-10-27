/*
 * [XSI] NLSPATH: %N stands for the name; the prefix and suffix of the template are kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catopen.006
 */
#include "cg.h"

int main(void)
{
	nl_catd cd;
	const char *m;

	pc_begin("XSH.catopen.006");
	cg_nlspath_guard();
	cg_make("pca6.cat", "c006.msg", CG_SRC);
	cg_setpath("./pc%N.cat");
	pc_test();
	errno = 0;
	cd = catopen("a6", 0);
	pc_check(cd != CG_BAD, "catopen(\"a6\") with NLSPATH=./pc%%N.cat failed: %s", pc_errname(errno));
	m = catgets(cd, 1, 1, "default");
	pc_check(strcmp(m, "Hello catalog") == 0, "message is \"%s\"", m);
	pc_check(catclose(cd) == 0, "catclose failed");
	pc_pass();
}
