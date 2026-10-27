/*
 * strcasecmp: order of the lowercased bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcasecmp.002
 */
#include "sc.h"

int main(void)
{
	pc_begin("XSH.strcasecmp.002");
	sc_init();
	pc_test();
	sc_order(0, 0, (locale_t)0);
	pc_pass();
}
