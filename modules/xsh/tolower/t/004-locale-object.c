/*
 * tolower: the _l form maps letters
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tolower.004
 */
#include "cv.h"

int main(void)
{
	locale_t loc = (locale_t)0;

	pc_begin("XSH.tolower.004");
	ct_init();
	loc = ct_cloc();
	pc_test();
	cv_map(0, 2, 3, loc);
	pc_pass();
}
