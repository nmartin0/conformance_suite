/*
 * iswupper: weof.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswupper.003
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswupper.003");
	pc_test();
	pc_check(iswupper(WEOF) == 0, "iswupper(WEOF) returned non-zero");
	pc_pass();
}
