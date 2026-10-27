/*
 * [XSI] NLSPATH: a file that is not a valid catalog is ignored and the search goes on.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catopen.009
 */
#include "cg.h"

int main(void)
{
	nl_catd cd;
	const char *m;

	pc_begin("XSH.catopen.009");
	cg_nlspath_guard();
	pc_make_dir("bad9");
	pc_make_dir("good9");
	pc_make_file("bad9/c009x", "\377\376 this is not a message catalog \001\002\003\n");
	cg_make("good9/c009x", "c009.msg", CG_SRC);
	cg_setpath("./bad9/%N:./good9/%N");
	pc_test();
	errno = 0;
	cd = catopen("c009x", 0);
	pc_check(cd != CG_BAD, "catopen did not skip the file that is not a catalog: %s", pc_errname(errno));
	m = catgets(cd, 4, 20, "default");
	pc_check(strcmp(m, "Twenty") == 0, "message is \"%s\" (the invalid file was used?)", m);
	pc_check(catclose(cd) == 0, "catclose failed");
	pc_pass();
}
