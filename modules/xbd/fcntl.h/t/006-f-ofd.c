/*
 * <fcntl.h> defines F_OFD_GETLK, F_OFD_SETLK and F_OFD_SETLKW, usable in #if.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.006
 */
#include "pc.h"
#include <fcntl.h>

int main(void)
{
	pc_begin("XBD.fcntl.h.006");
	pc_test();
#if !defined(F_OFD_GETLK)
	pc_fail("<fcntl.h> does not define F_OFD_GETLK");
#elif !defined(F_OFD_SETLK)
	pc_fail("<fcntl.h> does not define F_OFD_SETLK");
#elif !defined(F_OFD_SETLKW)
	pc_fail("<fcntl.h> does not define F_OFD_SETLKW");
#elif (F_OFD_GETLK) || (F_OFD_SETLK) || (F_OFD_SETLKW) || 1
	pc_pass();
#endif
}
