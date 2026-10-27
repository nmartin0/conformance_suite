/*
 * iswdigit: members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswdigit.001
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswdigit.001");
	wc_init();
	pc_test();
	wc_chk("iswdigit", iswdigit, C_DIGIT, 1, 1);
	pc_pass();
}
