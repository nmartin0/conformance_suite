/*
 * mprotect.c - conforming variants of mprotect(), for the variant checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <sys/mman.h>
#include "pcvariant.h"

/*
 * "extra_read": any protection other than PROT_NONE also allows reading
 * (PROT_WRITE alone gives PROT_READ|PROT_WRITE). mprotect() DESCRIPTION
 * lets an implementation permit accesses beyond those in prot, as long as
 * no write succeeds without PROT_WRITE and nothing succeeds with
 * PROT_NONE alone.
 */
int mprotect(void *addr, size_t len, int prot)
{
	const char *v = variant_of("mprotect.");
	REAL(mprotect);

	if (IS("extra_read") && prot != PROT_NONE)
		prot |= PROT_READ;
	return real_mprotect(addr, len, prot);
}
