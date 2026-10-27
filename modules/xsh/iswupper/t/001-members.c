/*
 * iswupper: members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswupper.001
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswupper.001");
	wc_init();
	pc_test();
	wc_chk("iswupper", iswupper, C_UPPER, 1, 1);
	pc_pass();
}
