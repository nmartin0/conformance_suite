/*
 * TMP_MAX on XSI systems.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdio.h.005
 */
#include <stdio.h>
#include "pc.h"

_Static_assert(TMP_MAX >= 10000, "TMP_MAX is below 10000");

int main(void)
{
	pc_begin("XBD.stdio.h.005");
	pc_test();
	pc_pass();
}
