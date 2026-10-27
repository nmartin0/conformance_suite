/*
 * iswspace: non members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswspace.002
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswspace.002");
	wc_init();
	pc_test();
	wc_chk("iswspace", iswspace, C_SPACE, 0, 0);
	pc_pass();
}
