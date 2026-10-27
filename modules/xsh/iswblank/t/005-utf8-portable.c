/*
 * iswblank: utf8 portable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswblank.005
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswblank.005");
	wc_init();
	wc_utf8();
	pc_test();
	wc_chk("iswblank", iswblank, C_BLANK, 1, 1);
	pc_pass();
}
