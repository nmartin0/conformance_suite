/*
 * reclen_t is an unsigned integer type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_types.h.007
 */
#include <sys/types.h>
#include "pc.h"

_Static_assert((reclen_t)7 % (reclen_t)3 == 1, "reclen_t is not an integer type");
_Static_assert((reclen_t)-1 > 0, "reclen_t is not an unsigned integer type");
static reclen_t v = 3;

int main(void)
{
	pc_begin("XBD.sys_types.h.007");
	pc_test();
	v = v * 2 + 1;
	pc_check(v == 7 && v * 1.5 > 10.0, "reclen_t does not behave as an arithmetic type");
	pc_pass();
}
