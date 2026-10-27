/*
 * After a failed dlopen() the first dlerror() gives text without a newline and the second gives a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dlerror.002
 */
#include "dlx.h"

int main(void)
{
	const char *e;

	pc_begin("XSH.dlerror.002");
	(void)dlerror();
	pc_check(dlopen(DL_MISSING, RTLD_NOW) == NULL, "dlopen of a missing file returned a handle");
	pc_test();
	e = dlerror();
	pc_check(dlx_text_ok(e), "dlerror() gave no text, an empty text, or text ending in a newline");
	pc_check(dlerror() == NULL, "the second dlerror() returned text again");
	pc_pass();
}
