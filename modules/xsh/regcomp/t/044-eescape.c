/*
 * A trailing backslash is rejected by regcomp.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.044
 */
#include "rx.h"

int main(void)
{
	pc_begin("XSH.regcomp.044");
	pc_test();
	rx_bad("a\\", 0);
	rx_bad("a\\", REG_EXTENDED);
	rx_bad("\\", 0);
	pc_pass();
}
