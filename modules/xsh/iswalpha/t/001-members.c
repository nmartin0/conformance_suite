/*
 * iswalpha: members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswalpha.001
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswalpha.001");
	wc_init();
	pc_test();
	wc_chk("iswalpha", iswalpha, C_ALPHA, 1, 1);
	pc_pass();
}
