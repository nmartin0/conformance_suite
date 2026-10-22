/*
 * <fcntl.h> defines O_CLOFORK, usable in #if.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.005
 */
#include "pc.h"
#include <fcntl.h>

int main(void)
{
	pc_begin("XBD.fcntl.h.005");
	pc_test();
#if !defined(O_CLOFORK)
	pc_fail("<fcntl.h> does not define O_CLOFORK");
#elif (O_CLOFORK) || 1
	pc_pass();
#endif
}
