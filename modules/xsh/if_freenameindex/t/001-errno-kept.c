/*
 * if_freenameindex() of a list returned by if_nameindex() leaves errno unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.if_freenameindex.001
 */
#include "if.h"

int main(void)
{
	struct if_nameindex *l;

	pc_begin("XSH.if_freenameindex.001");
	l = get_list();
	pc_test();
	errno = EDOM;
	if_freenameindex(l);
	pc_check(errno == EDOM, "errno changed to %s", pc_errname(errno));
	pc_pass();
}
