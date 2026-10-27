/*
 * towctrans: zero desc.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.towctrans.003
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i;
	wint_t s[5];
	pc_begin("XSH.towctrans.003");
	wc_init();
	s[0] = (wint_t)L'a';
	s[1] = (wint_t)L'A';
	s[2] = (wint_t)L'0';
	s[3] = (wint_t)L' ';
	s[4] = WEOF;
	pc_test();
	for (i = 0; i < 5; i++)
		pc_check(towctrans(s[i], (wctrans_t)0) == s[i], "towctrans() with a zero descriptor changed the argument %d", i);
	pc_pass();
}
