/*
 * strncasecmp: order of the lowercased bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncasecmp.003
 */
#include "sc.h"

int main(void)
{
	pc_begin("XSH.strncasecmp.003");
	sc_init();
	pc_test();
	sc_order(2, 40, (locale_t)0);
	pc_pass();
}
