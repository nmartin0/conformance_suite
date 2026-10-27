/*
 * toupper: the _l form leaves the rest alone
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.toupper.005
 */
#include "cv.h"

int main(void)
{
	locale_t loc = (locale_t)0;

	pc_begin("XSH.toupper.005");
	ct_init();
	loc = ct_cloc();
	pc_test();
	cv_same(1, 1, 2, 3, loc);
	pc_pass();
}
