/*
 * iswxdigit: utf8 portable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswxdigit.005
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswxdigit.005");
	wc_init();
	wc_utf8();
	pc_test();
	wc_chk("iswxdigit", iswxdigit, C_XDIGIT, 1, 1);
	wc_chk("iswxdigit", iswxdigit, C_XDIGIT, 0, 0);
	pc_pass();
}
