/*
 * <fcntl.h> defines FD_CLOFORK, usable in #if.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.002
 */
#include "pc.h"
#include <fcntl.h>

int main(void)
{
	pc_begin("XBD.fcntl.h.002");
	pc_test();
#if !defined(FD_CLOFORK)
	pc_fail("<fcntl.h> does not define FD_CLOFORK");
#elif (FD_CLOFORK) || 1
	pc_pass();
#endif
}
