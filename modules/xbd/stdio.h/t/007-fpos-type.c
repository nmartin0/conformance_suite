/*
 * fpos_t is a complete non-array object type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdio.h.007
 */
#include <stdio.h>
#include <string.h>
#include "pc.h"

_Static_assert(sizeof(fpos_t) > 0, "fpos_t has no size");

int main(void)
{
	pc_begin("XBD.stdio.h.007");
	pc_test();
	fpos_t a, b;

	/* Assigning one fpos_t to another compiles only for a non-array object type; padding bytes are not compared. */
	memset(&a, 0x5a, sizeof a);
	b = a;
	pc_check(sizeof b == sizeof a && sizeof b > 0, "fpos_t has no usable size");
	pc_pass();
}
