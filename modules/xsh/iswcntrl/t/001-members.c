/*
 * iswcntrl: members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswcntrl.001
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswcntrl.001");
	wc_init();
	pc_test();
	wc_chk("iswcntrl", iswcntrl, C_CNTRL, 1, 1);
	pc_pass();
}
