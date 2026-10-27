/*
 * toupper: the _l form maps letters
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.toupper.004
 */
#include "cv.h"

int main(void)
{
	locale_t loc = (locale_t)0;

	pc_begin("XSH.toupper.004");
	ct_init();
	loc = ct_cloc();
	pc_test();
	cv_map(1, 2, 3, loc);
	pc_pass();
}
