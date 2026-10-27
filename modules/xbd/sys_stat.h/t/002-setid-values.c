/*
 * S_ISUID and S_ISGID are usable in #if and have the octal values 04000
 * and 02000.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.002
 */
#include "pc.h"
#include <sys/stat.h>

int main(void)
{
	pc_begin("XBD.sys_stat.h.002");
#if !defined(S_ISUID) || !defined(S_ISGID)
	pc_unresolved("a constant is missing (the header check reports it): S_ISUID S_ISGID");
#else
	pc_test();
#if S_ISUID != 04000
	pc_fail("S_ISUID has the wrong value");
#elif S_ISGID != 02000
	pc_fail("S_ISGID has the wrong value");
#else
	pc_pass();
#endif
#endif
}
