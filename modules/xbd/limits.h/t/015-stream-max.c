/*
 * STREAM_MAX equals FOPEN_MAX.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.015
 */
#include <stdio.h>
#include <limits.h>
#include "pc.h"

#ifdef STREAM_MAX
_Static_assert(STREAM_MAX == FOPEN_MAX, "STREAM_MAX differs from FOPEN_MAX");
#endif

int main(void)
{
	pc_begin("XBD.limits.h.015");
	pc_test();
	pc_pass();
}
