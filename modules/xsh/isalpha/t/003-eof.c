/*
 * isalpha: EOF is in no class
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isalpha.003
 */
#include "ct.h"

int main(void)
{
	pc_begin("XSH.isalpha.003");
	ct_init();
	pc_test();
	ct_eof(C_ALPHA, 0, 1, (locale_t)0);
	pc_pass();
}
