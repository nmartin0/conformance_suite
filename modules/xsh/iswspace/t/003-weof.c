/*
 * iswspace: weof.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswspace.003
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswspace.003");
	pc_test();
	pc_check(iswspace(WEOF) == 0, "iswspace(WEOF) returned non-zero");
	pc_pass();
}
