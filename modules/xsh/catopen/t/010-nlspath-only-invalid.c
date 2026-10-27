/*
 * [XSI] NLSPATH: when only files that are not catalogs are found, catopen() fails with ENOENT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catopen.010
 */
#include "cg.h"

int main(void)
{
	nl_catd cd;

	pc_begin("XSH.catopen.010");
	cg_nlspath_guard();
	pc_make_dir("bad10");
	pc_make_file("bad10/pc-c010x", "\377\376 this is not a message catalog \001\002\003\n");
	cg_setpath("./bad10/%N");
	pc_test();
	errno = 0;
	cd = catopen("pc-c010x", 0);
	if (cd != CG_BAD) {
		(void)catclose(cd);
		pc_unresolved("the file was accepted as a catalog (the format is implementation-defined)");
	}
	pc_check(errno == ENOENT, "errno %s, expected ENOENT", pc_errname(errno));
	pc_pass();
}
