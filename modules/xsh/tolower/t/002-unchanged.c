/*
 * tolower: all other portable characters
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tolower.002
 */
#include "cv.h"

int main(void)
{
	pc_begin("XSH.tolower.002");
	ct_init();
	pc_test();
	cv_same(0, 0, 0, 1, (locale_t)0);
	pc_pass();
}
