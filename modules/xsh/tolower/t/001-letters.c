/*
 * tolower: letters of the source case
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tolower.001
 */
#include "cv.h"

int main(void)
{
	pc_begin("XSH.tolower.001");
	ct_init();
	pc_test();
	cv_map(0, 0, 1, (locale_t)0);
	pc_pass();
}
