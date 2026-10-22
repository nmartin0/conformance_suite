/*
 * <fcntl.h> defines O_CLOEXEC, usable in #if.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.004
 */
#include "pc.h"
#include <fcntl.h>

int main(void)
{
	pc_begin("XBD.fcntl.h.004");
	pc_test();
#if !defined(O_CLOEXEC)
	pc_fail("<fcntl.h> does not define O_CLOEXEC");
#elif (O_CLOEXEC) || 1
	pc_pass();
#endif
}
