/*
 * dlopen() of a pathname with no file returns a null pointer and dlerror() reports it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dlopen.002
 */
#include "dlx.h"

int main(void)
{
	void *h;

	pc_begin("XSH.dlopen.002");
	(void)dlerror();
	pc_test();
	h = dlopen(DL_MISSING, RTLD_NOW);
	pc_check(h == NULL, "dlopen of a missing file returned a handle");
	pc_check(dlx_text_ok(dlerror()), "dlerror() gave no diagnostic after the failure");
	pc_pass();
}
