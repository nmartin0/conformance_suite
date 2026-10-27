/*
 * isspace: members of the class
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.isspace.001
 */
#include "ct.h"

int main(void)
{
	pc_begin("XSH.isspace.001");
	ct_init();
	pc_test();
	ct_scan(C_SPACE, 1, 0, 1, (locale_t)0);
	pc_pass();
}
