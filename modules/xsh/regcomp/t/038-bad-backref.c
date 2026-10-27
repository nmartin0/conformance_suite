/*
 * A back-reference to a subexpression that does not precede it makes the BRE invalid: regcomp fails.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.038
 */
#include "rx.h"

int main(void)
{
	pc_begin("XSH.regcomp.038");
	pc_test();
	rx_bad("\\1", 0);
	rx_bad("a\\2", 0);
	rx_bad("\\(a\\)\\2", 0);
	rx_bad("\\(a\\)\\(b\\)\\3", 0);
	pc_pass();
}
