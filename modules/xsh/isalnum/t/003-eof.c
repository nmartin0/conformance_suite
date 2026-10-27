/*
 * isalnum: EOF is in no class
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isalnum.003
 */
#include "ct.h"

int main(void)
{
	pc_begin("XSH.isalnum.003");
	ct_init();
	pc_test();
	ct_eof(C_ALNUM, 0, 1, (locale_t)0);
	pc_pass();
}
