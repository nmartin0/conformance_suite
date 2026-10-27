/*
 * iswpunct: members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswpunct.001
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswpunct.001");
	wc_init();
	pc_test();
	wc_chk("iswpunct", iswpunct, C_PUNCT, 1, 1);
	pc_pass();
}
