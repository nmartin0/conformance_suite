/*
 * <fcntl.h> defines O_TTY_INIT, usable in #if (its value may be 0).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.007
 */
#include "pc.h"
#include <fcntl.h>

int main(void)
{
	pc_begin("XBD.fcntl.h.007");
	pc_test();
#if !defined(O_TTY_INIT)
	pc_fail("<fcntl.h> does not define O_TTY_INIT");
#elif (O_TTY_INIT) || 1
	pc_pass();
#endif
}
