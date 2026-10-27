/*
 * iswctype: zero class.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswctype.003
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i;
	wint_t s[5];
	pc_begin("XSH.iswctype.003");
	wc_init();
	s[0] = (wint_t)L'a';
	s[1] = (wint_t)L'A';
	s[2] = (wint_t)L'0';
	s[3] = (wint_t)L'!';
	s[4] = WEOF;
	pc_test();
	for (i = 0; i < 5; i++)
		pc_check(iswctype(s[i], (wctype_t)0) == 0, "iswctype() with a zero class returned non-zero for argument %d", i);
	pc_pass();
}
