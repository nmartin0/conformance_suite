/*
 * MAP_ANON has the same value as MAP_ANONYMOUS.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_mman.h.003
 */
#include "pc.h"
#include <sys/mman.h>

int main(void)
{
	pc_begin("XBD.sys_mman.h.003");
#if !defined(MAP_ANON) || !defined(MAP_ANONYMOUS)
	pc_unresolved("MAP_ANON or MAP_ANONYMOUS is missing (the header check reports it)");
#else
	pc_test();
	pc_check((MAP_ANON) == (MAP_ANONYMOUS), "MAP_ANON (%#x) differs from MAP_ANONYMOUS (%#x)", (unsigned)(MAP_ANON), (unsigned)(MAP_ANONYMOUS));
	pc_pass();
#endif
}
