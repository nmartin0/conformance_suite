/*
 * Successful dlopen(), dlsym() and dlclose() calls leave dlerror() with nothing to report.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dlerror.005
 */
#include "dlx.h"

int main(void)
{
	void *h, *s;

	pc_begin("XSH.dlerror.005");
	(void)dlerror();
	pc_test();
	h = dlopen(NULL, RTLD_NOW);
	pc_check(h != NULL, "dlopen(NULL) failed");
	s = dlsym(h, "getpid");
	if (s == NULL)
		pc_unresolved("getpid is not among the symbols the program image exports (the page leaves the set open)");
	pc_check(dlclose(h) == 0, "dlclose() failed");
	pc_check(dlerror() == NULL, "dlerror() returned text after calls that succeeded");
	pc_pass();
}
