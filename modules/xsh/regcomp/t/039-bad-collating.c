/*
 * A collating symbol naming no collating element of the locale makes the expression invalid: regcomp fails.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.039
 */
#include "rx.h"

int main(void)
{
	pc_begin("XSH.regcomp.039");
	pc_test();
	rx_bad("[[.xyzzy.]]", 0);
	rx_bad("[[.xyzzy.]]", REG_EXTENDED);
	rx_bad("[a[.qq.]]", 0);
	rx_bad("[[.zz.]-a]", REG_EXTENDED);
	pc_pass();
}
