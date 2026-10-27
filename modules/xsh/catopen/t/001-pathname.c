/*
 * A name with a slash is a pathname; the catalog made by gencat opens with either flag value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catopen.001
 */
#include "cg.h"

int main(void)
{
	nl_catd cd;
	const char *m;
	int flags[2];
	int i;

	flags[0] = 0;
	flags[1] = NL_CAT_LOCALE;
	pc_begin("XSH.catopen.001");
	cg_make("c001.cat", "c001.msg", CG_SRC);
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = 0;
		cd = catopen("./c001.cat", flags[i]);
		pc_check(cd != CG_BAD, "catopen(\"./c001.cat\", %d) failed: %s", flags[i], pc_errname(errno));
		m = catgets(cd, 1, 1, "default");
		pc_check(strcmp(m, "Hello catalog") == 0, "message 1 of set 1 is \"%s\"", m);
		pc_check(catclose(cd) == 0, "catclose failed");
	}
	pc_pass();
}
