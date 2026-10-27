/*
 * SHM_FAILED has the same value as ((void *)(intptr_t)-1).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_shm.h.003
 */
#include "pc.h"
#include <stdint.h>
#include <sys/shm.h>

int main(void)
{
	pc_begin("XBD.sys_shm.h.003");
#ifndef SHM_FAILED
	pc_unresolved("SHM_FAILED is missing (the header check reports it)");
#else
	pc_test();
	pc_check((SHM_FAILED) == ((void *)(intptr_t)-1), "SHM_FAILED differs from ((void *)(intptr_t)-1)");
	pc_pass();
#endif
}
