/*
 * sigaltstack.c - conforming variants of sigaltstack(), for the variant
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it. This file is built without the variant headers, so
 * SS_DISABLE and MINSIGSTKSZ here are the C library's own values.
 */
/*
 * The tests' feature macros, not _GNU_SOURCE: with it, glibc makes
 * MINSIGSTKSZ a run-time value that differs from the one the tests see.
 */
#define _XOPEN_SOURCE 800
#include <errno.h>
#include <signal.h>
#include <dlfcn.h>
#ifndef RTLD_NEXT
#define RTLD_NEXT ((void *)-1L)	/* glibc and musl, outside _GNU_SOURCE */
#endif
#include "pcvariant.h"

#define V_SS_DISABLE 4		/* sigaltstack.bsd_flags.h */
#define V_MINSIGSTKSZ 32768	/* sigaltstack.minsigstksz_32k.h */

int sigaltstack(const stack_t *ss, stack_t *oss)
{
	const char *v = variant_of("sigaltstack.");
	stack_t n;
	int r;
	REAL(sigaltstack);

	if (v == NULL)
		return real_sigaltstack(ss, oss);
	if (ss != NULL) {
		n = *ss;
		if (IS("bsd_flags")) {
			/*
			 * "bsd_flags": SS_DISABLE is 4 (see the header). The
			 * library's own SS_DISABLE value is then not a flag of
			 * this system, so it is an invalid flag (EINVAL).
			 */
			if (n.ss_flags & SS_DISABLE) {
				errno = EINVAL;
				return -1;
			}
			if (n.ss_flags & V_SS_DISABLE)
				n.ss_flags = (n.ss_flags & ~V_SS_DISABLE) | SS_DISABLE;
		}
		/*
		 * "minsigstksz_32k": MINSIGSTKSZ is 32768 (see the header), and
		 * an enabled stack smaller than that fails with ENOMEM.
		 */
		if (IS("minsigstksz_32k") && !(n.ss_flags & SS_DISABLE) && n.ss_size < V_MINSIGSTKSZ &&
		    !(n.ss_flags & ~(SS_DISABLE | SS_ONSTACK))) {
			errno = ENOMEM;
			return -1;
		}
		/*
		 * "disable_size_checked": a call with SS_DISABLE and a size
		 * below MINSIGSTKSZ fails with ENOMEM. The DESCRIPTION says the
		 * size is ignored with SS_DISABLE, but the ENOMEM condition has
		 * no exception for it (standard issue SI-47); a system reading
		 * ENOMEM as applying is planted here only for tests that do
		 * not rest on that point.
		 */
		if (IS("disable_size_checked") && (n.ss_flags & SS_DISABLE) && n.ss_size < (size_t)MINSIGSTKSZ) {
			errno = ENOMEM;
			return -1;
		}
		ss = &n;
	}
	r = real_sigaltstack(ss, oss);
	if (r == 0 && oss != NULL && IS("bsd_flags") && (oss->ss_flags & SS_DISABLE))
		oss->ss_flags = (oss->ss_flags & ~SS_DISABLE) | V_SS_DISABLE;
	return r;
}
