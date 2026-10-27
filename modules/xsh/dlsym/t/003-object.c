/*
 * dlsym() on a handle from dlopen(pathname) finds the symbols of that object.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dlsym.003
 */
#define DLX_OBJECT
#include "dlx.h"

int main(void)
{
	char path[4096];
	void *h, *s;

	pc_begin("XSH.dlsym.003");
	dlx_object(path, sizeof path);
	h = dlopen(path, RTLD_NOW);
	if (h == NULL)
		pc_unresolved("dlopen of %s failed: the kind of object is not eligible?", path);
	pc_test();
	s = dlsym(h, "getpid");
	pc_check(s != NULL, "dlsym() did not find getpid in %s", path);
	pc_check(dlx_getpid(s) == getpid(), "getpid through the address gave a wrong result");
	pc_check(dlsym(h, "pc_no_such_symbol_anywhere") == NULL, "dlsym() found a symbol that cannot exist");
	(void)dlclose(h);
	pc_pass();
}
