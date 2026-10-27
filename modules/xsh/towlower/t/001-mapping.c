/*
 * towlower: mapping.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.towlower.001
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i;
	wint_t r;
	pc_begin("XSH.towlower.001");
	wc_init();
	pc_test();
	for (i = 0; i < 128; i++) {
		if (!wc_in(C_UPPER, i))
			continue;
		r = towlower(wc_ch[i]);
		pc_check(r == wc_ch[i + 32], "towlower() of the character at position %d returned 0x%lx, expected the one at position %d", i, (unsigned long)r, i + 32);
	}
	pc_pass();
}
