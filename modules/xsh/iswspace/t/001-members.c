/*
 * iswspace: members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswspace.001
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswspace.001");
	wc_init();
	pc_test();
	wc_chk("iswspace", iswspace, C_SPACE, 1, 1);
	pc_pass();
}
