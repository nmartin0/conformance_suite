/*
 * tolower: the _l form leaves the rest alone
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tolower.005
 */
#include "cv.h"

int main(void)
{
	locale_t loc = (locale_t)0;

	pc_begin("XSH.tolower.005");
	ct_init();
	loc = ct_cloc();
	pc_test();
	cv_same(0, 1, 2, 3, loc);
	pc_pass();
}
