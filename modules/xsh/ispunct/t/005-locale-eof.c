/*
 * ispunct: the _l form with EOF
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ispunct.005
 */
#include "ct.h"

int main(void)
{
	locale_t loc = (locale_t)0;

	pc_begin("XSH.ispunct.005");
	ct_init();
	loc = ct_cloc();
	pc_test();
	ct_eof(C_PUNCT, 2, 3, loc);
	pc_pass();
}
