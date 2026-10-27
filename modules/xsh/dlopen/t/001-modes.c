/*
 * dlopen(NULL, mode) gives a handle for each combination of binding time and scope.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dlopen.001
 */
#include "dlx.h"

int main(void)
{
	static const int mode[4] = { RTLD_LAZY | RTLD_GLOBAL, RTLD_LAZY | RTLD_LOCAL, RTLD_NOW | RTLD_GLOBAL, RTLD_NOW | RTLD_LOCAL };
	void *h;
	int i;

	pc_begin("XSH.dlopen.001");
	pc_test();
	for (i = 0; i < 4; i++) {
		h = dlopen(NULL, mode[i]);
		pc_check(h != NULL, "dlopen(NULL, mode %d) returned a null pointer", i);
		(void)dlclose(h);
	}
	pc_pass();
}
