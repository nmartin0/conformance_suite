/*
 * SHMLBA is positive.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_shm.h.002
 */
#include "pc.h"
#include <sys/shm.h>

int main(void)
{
	pc_begin("XBD.sys_shm.h.002");
#ifndef SHMLBA
	pc_unresolved("SHMLBA is missing (the header check reports it)");
#else
	pc_test();
	/* shmat() takes an address modulo SHMLBA, so it must be positive. */
	pc_check((SHMLBA) > 0, "SHMLBA is not positive (%ld)", (long)(SHMLBA));
	pc_pass();
#endif
}
