/*
 * <unistd.h> defines POSIX_CLOSE_RESTART, usable in #if.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.001
 */
#include "pc.h"
#include <unistd.h>

int main(void)
{
	pc_begin("XBD.unistd.h.001");
	pc_test();
#if !defined(POSIX_CLOSE_RESTART)
	pc_fail("<unistd.h> does not define POSIX_CLOSE_RESTART");
#elif (POSIX_CLOSE_RESTART) || 1
	pc_pass();
#endif
}
