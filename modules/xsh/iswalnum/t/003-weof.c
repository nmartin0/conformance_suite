/*
 * iswalnum: weof.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswalnum.003
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswalnum.003");
	pc_test();
	pc_check(iswalnum(WEOF) == 0, "iswalnum(WEOF) returned non-zero");
	pc_pass();
}
