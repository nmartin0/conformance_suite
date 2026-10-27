/*
 * iswblank: weof.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswblank.003
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswblank.003");
	pc_test();
	pc_check(iswblank(WEOF) == 0, "iswblank(WEOF) returned non-zero");
	pc_pass();
}
