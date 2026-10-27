/*
 * getdate_err has type int.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.time.h.007
 */
#include <time.h>
#include "pc.h"

_Static_assert(_Generic(getdate_err, int: 1, default: 0), "getdate_err does not have type int");

int main(void)
{
	pc_begin("XBD.time.h.007");
	pc_test();
	int v = getdate_err;

	(void)v;
	pc_pass();
}
