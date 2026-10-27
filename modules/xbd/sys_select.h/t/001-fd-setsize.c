/*
 * FD_SETSIZE is usable in #if, positive, and fd_set has room for that many
 * descriptors.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_select.h.001
 */
#include "pc.h"
#include <limits.h>
#include <sys/select.h>

#if defined(FD_SETSIZE)
#if !((FD_SETSIZE) || 1)
#error "FD_SETSIZE is not usable in #if"
#endif
#endif

int main(void)
{
	pc_begin("XBD.sys_select.h.001");
#ifndef FD_SETSIZE
	pc_unresolved("FD_SETSIZE is missing (the header check reports it)");
#else
	pc_test();
	pc_check((FD_SETSIZE) > 0, "FD_SETSIZE is not positive (%ld)", (long)(FD_SETSIZE));
	pc_check(sizeof(fd_set) * CHAR_BIT >= (unsigned long)(FD_SETSIZE), "fd_set (%lu bits) cannot hold FD_SETSIZE (%ld) descriptors", (unsigned long)(sizeof(fd_set) * CHAR_BIT), (long)(FD_SETSIZE));
	pc_pass();
#endif
}
