/*
 * iswdigit: weof.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswdigit.003
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswdigit.003");
	pc_test();
	pc_check(iswdigit(WEOF) == 0, "iswdigit(WEOF) returned non-zero");
	pc_pass();
}
