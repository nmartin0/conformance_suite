/*
 * mbsinit() returns non-zero for a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbsinit.001
 */
#include "pc.h"
#include <wchar.h>

int main(void)
{
	int r;

	pc_begin("XSH.mbsinit.001");
	pc_test();
	r = mbsinit(NULL);
	pc_check(r != 0, "mbsinit(NULL) returned 0, expected non-zero");
	pc_pass();
}
