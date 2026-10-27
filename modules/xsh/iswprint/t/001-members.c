/*
 * iswprint: members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswprint.001
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswprint.001");
	wc_init();
	pc_test();
	wc_chk("iswprint", iswprint, C_PRINT, 1, 1);
	pc_pass();
}
