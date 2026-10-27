/*
 * FOPEN_MAX and TMP_MAX minimum values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdio.h.004
 */
#include <stdio.h>
#include "pc.h"

_Static_assert(FOPEN_MAX >= 8, "FOPEN_MAX is below 8");
_Static_assert(TMP_MAX >= 25, "TMP_MAX is below 25");

int main(void)
{
	pc_begin("XBD.stdio.h.004");
	pc_test();
	pc_pass();
}
