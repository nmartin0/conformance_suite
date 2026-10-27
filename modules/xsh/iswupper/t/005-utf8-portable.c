/*
 * iswupper: utf8 portable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswupper.005
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswupper.005");
	wc_init();
	wc_utf8();
	pc_test();
	wc_chk("iswupper", iswupper, C_UPPER, 1, 1);
	pc_pass();
}
