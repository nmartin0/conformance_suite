/*
 * Several errors before one dlerror() are reported by that call, and then nothing remains.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dlerror.004
 */
#include "dlx.h"

int main(void)
{
	void *h;

	pc_begin("XSH.dlerror.004");
	h = dlopen(NULL, RTLD_NOW);
	pc_check(h != NULL, "dlopen(NULL) failed");
	(void)dlerror();
	pc_check(dlopen(DL_MISSING, RTLD_NOW) == NULL, "dlopen of a missing file returned a handle");
	pc_check(dlsym(h, "pc_no_such_symbol_anywhere") == NULL, "dlsym() found a symbol that cannot exist");
	pc_test();
	pc_check(dlx_text_ok(dlerror()), "dlerror() gave no text after two errors");
	pc_check(dlerror() == NULL, "the second dlerror() returned text again");
	(void)dlclose(h);
	pc_pass();
}
