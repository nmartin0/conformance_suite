/*
 * The RTLD_ relocation modes differ, the visibility modes differ, and
 *   the two pairs can be OR-ed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.dlfcn.h.001
 */
#include <dlfcn.h>
#include "pc.h"


int main(void)
{
	pc_begin("XBD.dlfcn.h.001");
	pc_test();
#if !defined(RTLD_LAZY) || !defined(RTLD_NOW) || !defined(RTLD_GLOBAL) || !defined(RTLD_LOCAL)
	pc_fail("<dlfcn.h> does not define RTLD_LAZY, RTLD_NOW, RTLD_GLOBAL and RTLD_LOCAL");
#else
	pc_check(RTLD_LAZY != RTLD_NOW, "RTLD_LAZY and RTLD_NOW are both %d", (int)RTLD_LAZY);
	pc_check(RTLD_GLOBAL != RTLD_LOCAL, "RTLD_GLOBAL and RTLD_LOCAL are both %d", (int)RTLD_GLOBAL);
	pc_check((RTLD_LAZY & RTLD_GLOBAL) == 0, "RTLD_LAZY and RTLD_GLOBAL have bits in common");
	pc_check((RTLD_LAZY & RTLD_LOCAL) == 0, "RTLD_LAZY and RTLD_LOCAL have bits in common");
	pc_check((RTLD_NOW & RTLD_GLOBAL) == 0, "RTLD_NOW and RTLD_GLOBAL have bits in common");
	pc_check((RTLD_NOW & RTLD_LOCAL) == 0, "RTLD_NOW and RTLD_LOCAL have bits in common");
	pc_pass();
#endif
}
