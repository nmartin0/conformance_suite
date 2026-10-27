/*
 * dlopen() of a file that is not an executable object returns a null pointer and dlerror() reports it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dlopen.003
 */
#include "dlx.h"

int main(void)
{
	void *h;

	pc_begin("XSH.dlopen.003");
	pc_make_file("notobj.so", "this is not an executable object file\n");
	(void)dlerror();
	pc_test();
	h = dlopen("./notobj.so", RTLD_NOW);
	pc_check(h == NULL, "dlopen of a text file returned a handle");
	pc_check(dlx_text_ok(dlerror()), "dlerror() gave no diagnostic after the failure");
	pc_pass();
}
