/*
 * <fcntl.h> defines FD_CLOEXEC, usable in #if.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.001
 */
#include "pc.h"
#include <fcntl.h>

int main(void)
{
	pc_begin("XBD.fcntl.h.001");
	pc_test();
#if !defined(FD_CLOEXEC)
	pc_fail("<fcntl.h> does not define FD_CLOEXEC");
#elif (FD_CLOEXEC) || 1
	pc_pass();
#endif
}
