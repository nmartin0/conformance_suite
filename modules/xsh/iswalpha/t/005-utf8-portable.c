/*
 * iswalpha: utf8 portable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswalpha.005
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswalpha.005");
	wc_init();
	wc_utf8();
	pc_test();
	wc_chk("iswalpha", iswalpha, C_ALPHA, 1, 1);
	pc_pass();
}
