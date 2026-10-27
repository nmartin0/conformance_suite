/*
 * setutxent() returns to the first entry.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getutxent.002
 */
#include "utx.h"

int main(void)
{
	struct utmpx f, *p;

	pc_begin("XSH.getutxent.002");
	pc_test();
	setutxent();
	p = getutxent();
	if (p == NULL)
		pc_notinuse("the user accounting database is absent, empty or hidden");
	f = *p;
	(void)getutxent();
	(void)getutxent();
	setutxent();
	p = getutxent();
	pc_check(p != NULL, "no entry after setutxent()");
	pc_check(utx_same(&f, p), "the first entry after setutxent() is not the first entry");
	endutxent();
	pc_pass();
}
