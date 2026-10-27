/*
 * Closing one catalog leaves another open catalog usable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catclose.002
 */
#include "cg.h"

int main(void)
{
	nl_catd a, b;
	const char *m;

	pc_begin("XSH.catclose.002");
	cg_make("k2a.cat", "k2a.msg", "$set 1 a\n1 from a\n");
	cg_make("k2b.cat", "k2b.msg", "$set 1 b\n1 from b\n");
	a = catopen("./k2a.cat", 0);
	b = catopen("./k2b.cat", 0);
	pc_check(a != CG_BAD && b != CG_BAD, "catopen of the test catalogs failed: %s", pc_errname(errno));
	pc_test();
	pc_check(catclose(a) == 0, "catclose of the first catalog failed");
	m = catgets(b, 1, 1, "default message");
	pc_check(strcmp(m, "from b") == 0, "after closing the other catalog the message is \"%s\"", m);
	pc_check(catclose(b) == 0, "catclose of the second catalog failed");
	pc_pass();
}
