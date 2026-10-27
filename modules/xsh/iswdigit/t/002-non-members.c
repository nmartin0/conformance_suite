/*
 * iswdigit: non members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswdigit.002
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswdigit.002");
	wc_init();
	pc_test();
	wc_chk("iswdigit", iswdigit, C_DIGIT, 0, 0);
	pc_pass();
}
