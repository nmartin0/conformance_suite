/*
 * towlower: unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.towlower.002
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i;
	wint_t r;
	pc_begin("XSH.towlower.002");
	wc_init();
	pc_test();
	for (i = 0; i < 128; i++) {
		if (wc_in(C_UPPER, i))
			continue;
		r = towlower(wc_ch[i]);
		pc_check(r == wc_ch[i], "towlower() changed the character at position %d", i);
	}
	pc_pass();
}
