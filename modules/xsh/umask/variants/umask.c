/*
 * umask.c - conforming variants of umask(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <sys/stat.h>
#include "pcvariant.h"

/*
 * "extra_bits": the value returned has S_ISUID and S_ISGID set, whatever
 * the mask. XSH umask() RETURN VALUE fixes only the permission bits (and,
 * on XSI systems, a clear S_ISVTX); the other bits are unspecified, and
 * this system ignores them in cmask, so umask(returned value) still
 * restores the mask.
 * "stores_bits": the mask also keeps the bits of cmask other than the
 * permission bits and S_ISVTX, and umask() returns them. XSH umask()
 * DESCRIPTION leaves the meaning of those bits implementation-defined and
 * RETURN VALUE requires only that the returned value restores them; here
 * they have no effect on created files.
 */
static mode_t kept;	/* inherited by fork(), as the mask is */

mode_t umask(mode_t cmask)
{
	const char *v = variant_of("umask.");
	mode_t old, prev;
	REAL(umask);

	if (v == NULL)
		return real_umask(cmask);
	old = real_umask(cmask & 0777);
	if (IS("extra_bits"))
		return old | S_ISUID | S_ISGID;
	if (IS("stores_bits")) {
		prev = kept;
#ifdef S_ISVTX
		kept = cmask & ~(mode_t)(0777 | S_ISVTX);
#else
		kept = cmask & ~(mode_t)0777;
#endif
		return old | prev;
	}
	return old;
}
