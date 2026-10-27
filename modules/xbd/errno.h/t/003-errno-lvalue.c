/*
 * errno is a modifiable int lvalue.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.errno.h.003
 */
#include <errno.h>
#include "pc.h"

_Static_assert(_Generic(errno, int: 1, default: 0), "errno does not have type int");

int main(void)
{
	int *p;

	pc_begin("XBD.errno.h.003");
	pc_test();
	errno = 0;
	pc_check(errno == 0, "errno is not 0 after assigning 0");
	errno = 123;
	pc_check(errno == 123, "errno is not 123 after assigning 123");
	errno++;
	pc_check(errno == 124, "errno is not 124 after incrementing it");
	errno += 6;
	pc_check(errno == 130, "errno is not 130 after adding 6");
	p = &errno;
	*p = 7;
	pc_check(errno == 7, "a store through &errno did not change errno");
	errno = 9;
	pc_check(*p == 9, "a store to errno was not seen through &errno");
	pc_pass();
}
