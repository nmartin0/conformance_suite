/*
 * Unbalanced parentheses are rejected by regcomp.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.040
 */
#include "rx.h"

int main(void)
{
	pc_begin("XSH.regcomp.040");
	pc_test();
	rx_bad("\\(a", 0);
	rx_bad("\\(\\(a\\)", 0);
	rx_bad("(a", REG_EXTENDED);
	rx_bad("((a)", REG_EXTENDED);
	pc_pass();
}
