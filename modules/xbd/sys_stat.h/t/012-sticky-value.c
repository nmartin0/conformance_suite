/*
 * On XSI systems S_ISVTX is usable in #if and has the octal value 01000.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.012
 */
#include "pc.h"
#include <sys/stat.h>

int main(void)
{
	pc_begin("XBD.sys_stat.h.012");
#if !defined(S_ISVTX)
	pc_unresolved("a constant is missing (the header check reports it): S_ISVTX");
#else
	pc_test();
#if S_ISVTX != 01000
	pc_fail("S_ISVTX has the wrong value");
#else
	pc_pass();
#endif
#endif
}
