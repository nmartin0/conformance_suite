/*
 * errno is a modifiable int lvalue.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.errno.001
 */
#include "pc.h"
#include <errno.h>

int main(void)
{
	int *p;

	pc_begin("XSH.errno.001");
	pc_test();
	errno = ERANGE;
	pc_check(errno == ERANGE, "errno reads %d after assigning ERANGE", errno);
	errno = 0;
	pc_check(errno == 0, "errno reads %d after assigning 0", errno);
	errno = EDOM;
	p = &errno;
	pc_check(*p == EDOM, "the address of errno does not reach the value");
	*p = EINVAL;
	pc_check(errno == EINVAL, "an assignment through the address of errno did not change errno");
	errno = 5;
	errno++;
	pc_check(errno == 6, "errno++ gave %d", errno);
	pc_check(_Generic(errno, int: 1, default: 0) == 1, "errno does not have type int");
	pc_pass();
}
