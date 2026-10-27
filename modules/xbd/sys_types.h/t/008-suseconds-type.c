/*
 * suseconds_t is an arithmetic type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_types.h.008
 */
#include <sys/types.h>
#include "pc.h"

static suseconds_t v = 3;

int main(void)
{
	pc_begin("XBD.sys_types.h.008");
	pc_test();
	v = v * 2 + 1;
	pc_check(v == 7 && v * 1.5 > 10.0, "suseconds_t does not behave as an arithmetic type");
	pc_pass();
}
