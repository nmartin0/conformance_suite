/*
 * After endutxent() the next getutxent() opens the database again at the first entry.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getutxent.006
 */
#include "utx.h"

int main(void)
{
	struct utmpx f, *p;

	pc_begin("XSH.getutxent.006");
	pc_test();
	setutxent();
	p = getutxent();
	if (p == NULL)
		pc_notinuse("the user accounting database is absent, empty or hidden");
	f = *p;
	(void)getutxent();
	endutxent();
	p = getutxent();
	pc_check(p != NULL, "no entry after endutxent()");
	pc_check(utx_same(&f, p), "the first entry after endutxent() is not the first entry");
	endutxent();
	pc_pass();
}
