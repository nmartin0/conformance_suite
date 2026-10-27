/*
 * iswlower: members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswlower.001
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswlower.001");
	wc_init();
	pc_test();
	wc_chk("iswlower", iswlower, C_LOWER, 1, 1);
	pc_pass();
}
