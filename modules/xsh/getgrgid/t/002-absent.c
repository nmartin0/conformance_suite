/*
 * An ID with no entry gives a null pointer and leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getgrgid.002
 */
#include "grdb.h"

int main(void)
{
	xid_t id;
	xent_t *p;

	pc_begin("XSH.getgrgid.002");
	id = xc_absent_id();
	pc_test();
	errno = XSENTINEL;
	p = getgrgid(id);
	if (p != NULL)
		pc_unresolved("an entry with ID %ld exists", (long)id);
	if (errno != XSENTINEL) {
		if (xc_mayfail(errno))
			pc_unresolved("the lookup reported %s", pc_errname(errno));
		pc_fail("errno changed to %s by a lookup that found nothing", pc_errname(errno));
	}
	pc_pass();
}
