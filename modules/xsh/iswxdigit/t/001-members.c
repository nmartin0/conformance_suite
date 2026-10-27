/*
 * iswxdigit: members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswxdigit.001
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswxdigit.001");
	wc_init();
	pc_test();
	wc_chk("iswxdigit", iswxdigit, C_XDIGIT, 1, 1);
	pc_pass();
}
