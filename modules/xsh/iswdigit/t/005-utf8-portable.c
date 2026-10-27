/*
 * iswdigit: utf8 portable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswdigit.005
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswdigit.005");
	wc_init();
	wc_utf8();
	pc_test();
	wc_chk("iswdigit", iswdigit, C_DIGIT, 1, 1);
	wc_chk("iswdigit", iswdigit, C_DIGIT, 0, 0);
	pc_pass();
}
