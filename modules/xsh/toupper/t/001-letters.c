/*
 * toupper: letters of the source case
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.toupper.001
 */
#include "cv.h"

int main(void)
{
	pc_begin("XSH.toupper.001");
	ct_init();
	pc_test();
	cv_map(1, 0, 1, (locale_t)0);
	pc_pass();
}
