/*
 * After a failed dlsym() the first dlerror() gives text without a newline and the second gives a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dlerror.003
 */
#include "dlx.h"

int main(void)
{
	void *h;
	const char *e;

	pc_begin("XSH.dlerror.003");
	h = dlopen(NULL, RTLD_NOW);
	pc_check(h != NULL, "dlopen(NULL) failed");
	(void)dlerror();
	pc_check(dlsym(h, "pc_no_such_symbol_anywhere") == NULL, "dlsym() found a symbol that cannot exist");
	pc_test();
	e = dlerror();
	pc_check(dlx_text_ok(e), "dlerror() gave no text, an empty text, or text ending in a newline");
	pc_check(dlerror() == NULL, "the second dlerror() returned text again");
	(void)dlclose(h);
	pc_pass();
}
