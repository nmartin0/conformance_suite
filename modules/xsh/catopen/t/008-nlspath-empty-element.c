/*
 * [XSI] NLSPATH: an empty element (leading, trailing or between colons) means %N.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catopen.008
 */
#include "cg.h"

int main(void)
{
	static const char *const paths[] = { ":pc-no-dir/%N", "pc-no-dir/%N:", "pc-no-a/%N::pc-no-b/%N" };
	nl_catd cd;
	const char *m;
	size_t i;

	pc_begin("XSH.catopen.008");
	cg_nlspath_guard();
	cg_make("c008x", "c008.msg", CG_SRC);
	pc_test();
	for (i = 0; i < sizeof paths / sizeof paths[0]; i++) {
		cg_setpath(paths[i]);
		errno = 0;
		cd = catopen("c008x", 0);
		pc_check(cd != CG_BAD, "NLSPATH=\"%s\": catopen failed: %s", paths[i], pc_errname(errno));
		m = catgets(cd, 1, 2, "default");
		pc_check(strcmp(m, "Second line") == 0, "NLSPATH=\"%s\": message is \"%s\"", paths[i], m);
		pc_check(catclose(cd) == 0, "catclose failed");
	}
	pc_pass();
}
