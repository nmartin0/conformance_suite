/*
 * iswpunct: weof.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswpunct.003
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswpunct.003");
	pc_test();
	pc_check(iswpunct(WEOF) == 0, "iswpunct(WEOF) returned non-zero");
	pc_pass();
}
