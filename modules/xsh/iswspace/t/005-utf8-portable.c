/*
 * iswspace: utf8 portable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswspace.005
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswspace.005");
	wc_init();
	wc_utf8();
	pc_test();
	wc_chk("iswspace", iswspace, C_SPACE, 1, 1);
	pc_pass();
}
