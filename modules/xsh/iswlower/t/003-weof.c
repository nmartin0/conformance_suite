/*
 * iswlower: weof.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswlower.003
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswlower.003");
	pc_test();
	pc_check(iswlower(WEOF) == 0, "iswlower(WEOF) returned non-zero");
	pc_pass();
}
