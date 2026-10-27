/*
 * iswcntrl: non members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswcntrl.002
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswcntrl.002");
	wc_init();
	pc_test();
	wc_chk("iswcntrl", iswcntrl, C_CNTRL, 0, 0);
	pc_pass();
}
