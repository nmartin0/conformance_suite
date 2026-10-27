/*
 * iswblank: members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswblank.001
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswblank.001");
	wc_init();
	pc_test();
	wc_chk("iswblank", iswblank, C_BLANK, 1, 1);
	pc_pass();
}
