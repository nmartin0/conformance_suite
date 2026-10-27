/*
 * A name with no entry gives a null pointer and leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getgrnam.002
 */
#include "grdb.h"

int main(void)
{
	char name[64];
	xent_t *p;

	pc_begin("XSH.getgrnam.002");
	snprintf(name, sizeof name, "pcnoent%ldx", (long)getpid());
	pc_test();
	errno = XSENTINEL;
	p = getgrnam(name);
	if (p != NULL)
		pc_unresolved("an entry named %s exists", name);
	if (errno != XSENTINEL) {
		if (xc_mayfail(errno))
			pc_unresolved("the lookup reported %s", pc_errname(errno));
		pc_fail("errno changed to %s by a lookup that found nothing", pc_errname(errno));
	}
	pc_pass();
}
