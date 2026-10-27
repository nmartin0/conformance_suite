/*
 * iswcntrl: weof.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswcntrl.003
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswcntrl.003");
	pc_test();
	pc_check(iswcntrl(WEOF) == 0, "iswcntrl(WEOF) returned non-zero");
	pc_pass();
}
