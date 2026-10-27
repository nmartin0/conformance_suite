/*
 * isalnum: members of the class
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isalnum.001
 */
#include "ct.h"

int main(void)
{
	pc_begin("XSH.isalnum.001");
	ct_init();
	pc_test();
	ct_scan(C_ALNUM, 1, 0, 1, (locale_t)0);
	pc_pass();
}
