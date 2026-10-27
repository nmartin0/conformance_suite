/*
 * MAP_FAILED has type void *.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_mman.h.006
 */
#include "pc.h"
#include <sys/mman.h>

int main(void)
{
	pc_begin("XBD.sys_mman.h.006");
#ifndef MAP_FAILED
	pc_unresolved("MAP_FAILED is missing (the header check reports it)");
#else
	pc_test();
	pc_check(_Generic((MAP_FAILED), void *: 1, default: 0), "MAP_FAILED does not have type void *");
	pc_pass();
#endif
}
