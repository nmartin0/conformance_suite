/*
 * dlclose() of a handle from dlopen(NULL) returns 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dlclose.001
 */
#include "dlx.h"

int main(void)
{
	void *h;

	pc_begin("XSH.dlclose.001");
	h = dlopen(NULL, RTLD_NOW);
	pc_check(h != NULL, "dlopen(NULL) failed");
	pc_test();
	pc_check(dlclose(h) == 0, "dlclose() of a valid handle did not return 0");
	pc_pass();
}
