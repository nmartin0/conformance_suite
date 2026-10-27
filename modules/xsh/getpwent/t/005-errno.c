/*
 * The set and end functions leave errno alone when they succeed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getpwent.005
 */
#include "pwdb.h"

static void check(const char *what, int e)
{
	if (e == XSENTINEL)
		return;
	if (xc_mayfail(e))
		pc_unresolved("%s reported %s", what, pc_errname(e));
	pc_fail("%s changed errno to %s", what, pc_errname(e));
}

int main(void)
{
	pc_begin("XSH.getpwent.005");
	pc_test();
	errno = XSENTINEL;
	setpwent();
	check("the set function", errno);
	(void)getpwent();
	errno = XSENTINEL;
	setpwent();
	check("the set function (database open)", errno);
	errno = XSENTINEL;
	endpwent();
	check("the end function", errno);
	errno = XSENTINEL;
	endpwent();
	check("the end function (database closed)", errno);
	pc_pass();
}
