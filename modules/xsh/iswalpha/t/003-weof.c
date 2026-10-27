/*
 * iswalpha: weof.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswalpha.003
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswalpha.003");
	pc_test();
	pc_check(iswalpha(WEOF) == 0, "iswalpha(WEOF) returned non-zero");
	pc_pass();
}
