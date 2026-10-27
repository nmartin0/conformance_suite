/*
 * iswlower: non members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswlower.002
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswlower.002");
	wc_init();
	pc_test();
	wc_chk("iswlower", iswlower, C_LOWER, 0, 0);
	pc_pass();
}
