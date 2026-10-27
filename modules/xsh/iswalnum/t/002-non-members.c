/*
 * iswalnum: non members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswalnum.002
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswalnum.002");
	wc_init();
	pc_test();
	wc_chk("iswalnum", iswalnum, C_ALNUM, 0, 0);
	pc_pass();
}
