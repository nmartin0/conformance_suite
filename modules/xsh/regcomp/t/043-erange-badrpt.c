/*
 * A reversed range and a repetition operator with nothing to repeat are rejected by regcomp.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.043
 */
#include "rx.h"

int main(void)
{
	pc_begin("XSH.regcomp.043");
	pc_test();
	rx_bad("[b-a]", 0);
	rx_bad("[b-a]", REG_EXTENDED);
	rx_bad("*a", REG_EXTENDED);
	rx_bad("+a", REG_EXTENDED);
	rx_bad("?a", REG_EXTENDED);
	rx_bad("a|*b", REG_EXTENDED);
	pc_pass();
}
