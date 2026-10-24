/*
 * pcvariant.h - helpers for conforming variants (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A conforming variant is behaviour the standard allows that the systems
 * the suite is tried on may not show: another permitted errno, a constant
 * with another permitted value, a late signal, a short transfer. It is
 * planted like a fault (a preload library, and optionally a header
 * included before the test), and the test must still PASS (ci/mutate.sh).
 *
 * Unlike a fault, a variant models the whole system, so it applies in
 * every phase and in every process of the test. Each variant must be
 * justified by the text in a comment next to it: a variant is never
 * written to match what some system happens to do.
 *
 * Use in modules/xsh/<function>/variants/<function>.c:
 *	#include "pcvariant.h"
 *	... const char *v = variant_of("futimens."); if (IS("utime_now_m1")) ...
 */
#ifndef PCVARIANT_H
#define PCVARIANT_H

#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

/* The variant's name after prefix, or NULL when no variant of prefix is active. */
static const char *variant_of(const char *prefix)
{
	const char *v = getenv("PC_VARIANT");
	size_t n = strlen(prefix);

	if (v == NULL || strncmp(v, prefix, n) != 0)
		return NULL;
	return v + n;
}

#define IS(name) (v != NULL && strcmp(v, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

#endif
