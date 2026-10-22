/*
 * FD_CLOEXEC and FD_CLOFORK are bitwise-distinct.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.003
 */
#include "pc.h"
#include <fcntl.h>

int main(void)
{
	pc_begin("XBD.fcntl.h.003");
#if !defined(FD_CLOEXEC) || !defined(FD_CLOFORK)
	pc_unresolved("FD_CLOEXEC or FD_CLOFORK is missing");
#else
	pc_test();
	pc_check((FD_CLOEXEC & FD_CLOFORK) == 0,
	    "FD_CLOEXEC (%#x) and FD_CLOFORK (%#x) share bits",
	    (unsigned)FD_CLOEXEC, (unsigned)FD_CLOFORK);
	pc_pass();
#endif
}
