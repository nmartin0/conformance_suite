/*
 * dlsym() of a name that is not defined returns a null pointer and dlerror() reports it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dlsym.002
 */
#include "dlx.h"

int main(void)
{
	void *h;

	pc_begin("XSH.dlsym.002");
	h = dlopen(NULL, RTLD_NOW);
	pc_check(h != NULL, "dlopen(NULL) failed");
	(void)dlerror();
	pc_test();
	pc_check(dlsym(h, "pc_no_such_symbol_anywhere") == NULL, "dlsym() found a symbol that cannot exist");
	pc_check(dlx_text_ok(dlerror()), "dlerror() gave no diagnostic after the failure");
	(void)dlclose(h);
	pc_pass();
}
