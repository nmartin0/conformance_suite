/*
 * _XOPEN_VERSION and _XOPEN_UNIX on XSI systems.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.005
 */
#include <unistd.h>
#include "pc.h"

#if !defined(_XOPEN_VERSION) || _XOPEN_VERSION != 800
#error "_XOPEN_VERSION is not defined as 800"
#endif
#if !defined(_XOPEN_UNIX) || _XOPEN_UNIX == -1
#error "_XOPEN_UNIX is not defined with a value other than -1"
#endif

int main(void)
{
	pc_begin("XBD.unistd.h.005");
	pc_test();
	pc_pass();
}
