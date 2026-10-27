/*
 * toupper: all other portable characters
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.toupper.002
 */
#include "cv.h"

int main(void)
{
	pc_begin("XSH.toupper.002");
	ct_init();
	pc_test();
	cv_same(1, 0, 0, 1, (locale_t)0);
	pc_pass();
}
