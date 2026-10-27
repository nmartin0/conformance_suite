/*
 * A second dlerror() with nothing in between returns a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dlerror.001
 */
#include "dlx.h"

int main(void)
{
	pc_begin("XSH.dlerror.001");
	(void)dlerror();
	pc_test();
	pc_check(dlerror() == NULL, "dlerror() returned an error text when no error occurred since the last call");
	pc_check(dlerror() == NULL, "dlerror() returned an error text on a third call");
	pc_pass();
}
