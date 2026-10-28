/*
 * Interval counts up to RE_DUP_MAX in an ERE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.4-ere.007
 */
#include "dupmax.h"

int main(void)
{
	pc_begin("XBD.9.4-ere.007");
	pc_test();
	dupmax_run(REG_EXTENDED);
	pc_pass();
}
