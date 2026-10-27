/*
 * iswprint: non members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswprint.002
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswprint.002");
	wc_init();
	pc_test();
	wc_chk("iswprint", iswprint, C_PRINT, 0, 0);
	pc_pass();
}
