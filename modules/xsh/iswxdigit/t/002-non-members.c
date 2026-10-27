/*
 * iswxdigit: non members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswxdigit.002
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswxdigit.002");
	wc_init();
	pc_test();
	wc_chk("iswxdigit", iswxdigit, C_XDIGIT, 0, 0);
	pc_pass();
}
