/*
 * Malformed interval expressions are rejected by regcomp.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.041
 */
#include "rx.h"

int main(void)
{
	pc_begin("XSH.regcomp.041");
	pc_test();
	rx_bad("a\\{1", 0);
	rx_bad("a\\{1,2", 0);
	rx_bad("a\\{x\\}", 0);
	rx_bad("a\\{2,1\\}", 0);
	rx_bad("a{2,1}", REG_EXTENDED);
	pc_pass();
}
