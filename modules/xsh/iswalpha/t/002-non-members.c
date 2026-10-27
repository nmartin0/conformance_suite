/*
 * iswalpha: non members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswalpha.002
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswalpha.002");
	wc_init();
	pc_test();
	wc_chk("iswalpha", iswalpha, C_ALPHA, 0, 0);
	pc_pass();
}
