/*
 * mmap.c - conforming variants of mmap(), for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "pcvariant.h"

/*
 * "extra_read": every mapping that allows any access also allows reading
 * (PROT_WRITE alone maps as PROT_READ|PROT_WRITE). mmap() DESCRIPTION lets
 * an implementation permit accesses beyond those in prot, as long as no
 * write succeeds without PROT_WRITE and nothing succeeds with PROT_NONE.
 *
 * "no_private": MAP_PRIVATE is not supported and fails with ENOTSUP.
 * mmap() ERRORS lists ENOTSUP for a MAP_PRIVATE request on an
 * implementation that does not support it (the ISO POSIX-1:1996 case in
 * the CHANGE HISTORY).
 *
 * "fixed_einval": every MAP_FIXED request fails with EINVAL. mmap()
 * DESCRIPTION allows MAP_FAILED with EINVAL whenever MAP_FIXED is set.
 *
 * "far_hint": a non-zero addr without MAP_FIXED is not used at all.
 * mmap() DESCRIPTION leaves how addr is used to the implementation; it is
 * only a suggestion.
 *
 * "pipe_enxio": mapping a pipe (or other FIFO) fails with ENXIO, as on an
 * implementation that holds no range of offsets valid for such an object.
 * mmap() ERRORS requires ENXIO when the range [off,off+len) is invalid for
 * the object, and ENODEV when its file type is not supported; XSH 2.3
 * allows either where both apply.
 */
void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off)
{
	const char *v = variant_of("mmap.");
	REAL(mmap);

	if (IS("extra_read") && prot != PROT_NONE)
		prot |= PROT_READ;
	if (IS("no_private") && (flags & MAP_PRIVATE)) {
		errno = ENOTSUP;
		return MAP_FAILED;
	}
	if (IS("fixed_einval") && (flags & MAP_FIXED)) {
		errno = EINVAL;
		return MAP_FAILED;
	}
	if (IS("pipe_enxio") && !(flags & MAP_ANONYMOUS)) {
		struct stat st;

		if (fstat(fd, &st) == 0 && S_ISFIFO(st.st_mode)) {
			errno = ENXIO;
			return MAP_FAILED;
		}
	}
	if (IS("far_hint") && !(flags & MAP_FIXED))
		addr = NULL;
	return real_mmap(addr, len, prot, flags, fd, off);
}
