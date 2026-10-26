/*
 * msync.c - conforming variants of msync(), for the variant checks (CI
 * only).
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
 * "flags_lenient" (READING of XSH.msync.002, .007 and .008): a flags value
 * with both MS_ASYNC and MS_SYNC, or with neither, is accepted and the call
 * succeeds (both: as MS_SYNC; neither: as MS_ASYNC, with MS_INVALIDATE kept).
 * This reads msync() DESCRIPTION's demand for exactly one of the two as a
 * requirement on the application, so a call that breaks it is undefined,
 * and not as defining the invalid flags value of ERRORS (candidate issue,
 * msync flags).
 */
int msync(void *addr, size_t len, int flags)
{
	const char *v = variant_of("msync.");
	REAL(msync);

	if (IS("flags_lenient")) {
		if ((flags & MS_ASYNC) && (flags & MS_SYNC))
			flags &= ~MS_ASYNC;
		else if (!(flags & (MS_ASYNC | MS_SYNC)))
			flags |= MS_ASYNC;
	}
	return real_msync(addr, len, flags);
}
