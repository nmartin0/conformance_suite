/*
 * dlsym() on the handle for the program finds functions that the program refers to.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dlsym.001
 */
#include "dlx.h"

int main(void)
{
	void *h, *s;

	pc_begin("XSH.dlsym.001");
	h = dlopen(NULL, RTLD_NOW);
	pc_check(h != NULL, "dlopen(NULL) failed");
	pc_check(getpid() > 0, "getpid() is not positive");
	pc_test();
	s = dlsym(h, "getpid");
	if (s == NULL)
		pc_unresolved("getpid is not among the symbols the program image exports (the page leaves the set open)");
	pc_check(dlx_getpid(s) == getpid(), "getpid through the address gave a wrong result");
	s = dlsym(h, "getppid");
	if (s == NULL)
		pc_unresolved("getppid is not among the symbols the program image exports (the page leaves the set open)");
	pc_check(dlx_getpid(s) == getppid(), "getppid through the address gave a wrong result");
	(void)dlclose(h);
	pc_pass();
}
