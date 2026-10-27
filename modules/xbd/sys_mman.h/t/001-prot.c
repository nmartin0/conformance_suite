/*
 * PROT_READ, PROT_WRITE and PROT_EXEC are nonzero, PROT_WRITE shares no
 * bits with the other two, and PROT_NONE differs from every OR of them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_mman.h.001
 */
#include "pc.h"
#include <sys/mman.h>

int main(void)
{
	pc_begin("XBD.sys_mman.h.001");
#if !defined(PROT_NONE) || !defined(PROT_READ) || !defined(PROT_WRITE) || !defined(PROT_EXEC)
	pc_unresolved("a PROT_ constant is missing (the header check reports it)");
#else
	unsigned long long r = (unsigned long long)(PROT_READ), w = (unsigned long long)(PROT_WRITE);
	unsigned long long x = (unsigned long long)(PROT_EXEC), none = (unsigned long long)(PROT_NONE);
	int s;

	pc_test();
	pc_check(r != 0 && w != 0 && x != 0, "PROT_READ, PROT_WRITE or PROT_EXEC is zero");
	/* Read and execute may share bits (more access than asked for is allowed); write may not. */
	pc_check((r & w) == 0 && (w & x) == 0, "PROT_WRITE has bits in common with PROT_READ or PROT_EXEC");
	/* PROT_NONE is a protection of its own: no OR of the other three may equal it. */
	for (s = 1; s < 8; s++) {
		unsigned long long v = ((s & 1) ? r : 0) | ((s & 2) ? w : 0) | ((s & 4) ? x : 0);

		pc_check(none != v, "PROT_NONE equals the OR of some of PROT_READ, PROT_WRITE and PROT_EXEC (%#llx)", v);
	}
	pc_pass();
#endif
}
