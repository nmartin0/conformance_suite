/*
 * errno is defined as a macro.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.errno.005
 */
#include "pc.h"
#include <errno.h>

int main(void)
{
	pc_begin("XSH.errno.005");
	pc_test();
#ifdef errno
	pc_pass();
#else
	pc_fail("errno is not defined as a macro by <errno.h>");
#endif
}
