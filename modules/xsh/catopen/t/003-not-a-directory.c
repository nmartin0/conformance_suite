/*
 * A path prefix that is a regular file fails with ENOTDIR (or ENOENT).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catopen.003
 */
#include "cg.h"

int main(void)
{
	nl_catd cd;
	const char *m;

	pc_begin("XSH.catopen.003");
	pc_make_file("c003.reg", "plain file\n");
	pc_test();
	errno = 0;
	cd = catopen("./c003.reg/cat", 0);
	if (cd == CG_BAD) {
		pc_check(errno == ENOTDIR || errno == ENOENT, "errno %s, expected ENOTDIR or ENOENT", pc_errname(errno));
		pc_pass();
	}
	m = catgets(cd, 1, 1, "default");
	pc_check(strcmp(m, "default") == 0, "catgets gave \"%s\"", m);
	pc_check(catclose(cd) == 0, "catclose failed");
	pc_pass();
}
