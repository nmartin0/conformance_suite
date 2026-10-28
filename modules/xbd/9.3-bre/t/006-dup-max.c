/*
 * Interval counts up to RE_DUP_MAX in a BRE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.3-bre.006
 */
#include "dupmax.h"

int main(void)
{
	pc_begin("XBD.9.3-bre.006");
	pc_test();
	dupmax_run(0);
	pc_pass();
}
