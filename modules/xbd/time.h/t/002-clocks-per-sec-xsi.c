/*
 * CLOCKS_PER_SEC is 1000000 on XSI systems.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.time.h.002
 */
#include <time.h>
#include "pc.h"

int main(void)
{
	pc_begin("XBD.time.h.002");
	pc_test();
	pc_check(CLOCKS_PER_SEC == 1000000, "CLOCKS_PER_SEC is not 1000000");
	pc_pass();
}
