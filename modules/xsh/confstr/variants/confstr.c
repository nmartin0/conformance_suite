/*
 * confstr.c - conforming variants of confstr(), for the variant checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "no_value": the obsolescent Issue 7 programming-environment names have
 * no configuration-defined value here: confstr() returns 0 and leaves
 * errno alone. XSH confstr() RETURN VALUE provides for a supported name
 * without a value, and <unistd.h> gives no value requirement for these
 * names.
 * "errno_on_success": a call that returns a value other than 0 leaves
 * errno at ENOENT. XSH 2.3 leaves errno unspecified after a successful
 * call, and confstr() promises an unchanged errno only when it returns 0
 * for a name without a value.
 */
static int is_v7(int name)
{
	switch (name) {
#ifdef _CS_POSIX_V7_ILP32_OFF32_CFLAGS
	case _CS_POSIX_V7_ILP32_OFF32_CFLAGS: case _CS_POSIX_V7_ILP32_OFF32_LDFLAGS: case _CS_POSIX_V7_ILP32_OFF32_LIBS:
	case _CS_POSIX_V7_ILP32_OFFBIG_CFLAGS: case _CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS: case _CS_POSIX_V7_ILP32_OFFBIG_LIBS:
	case _CS_POSIX_V7_LP64_OFF64_CFLAGS: case _CS_POSIX_V7_LP64_OFF64_LDFLAGS: case _CS_POSIX_V7_LP64_OFF64_LIBS:
	case _CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS: case _CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS: case _CS_POSIX_V7_LPBIG_OFFBIG_LIBS:
		return 1;
#endif
#ifdef _CS_POSIX_V7_WIDTH_RESTRICTED_ENVS
	case _CS_POSIX_V7_WIDTH_RESTRICTED_ENVS:
		return 1;
#endif
#ifdef _CS_V7_ENV
	case _CS_V7_ENV:
		return 1;
#endif
	}
	return 0;
}

size_t confstr(int name, char *buf, size_t len)
{
	const char *v = variant_of("confstr.");
	size_t n;
	REAL(confstr);

	if (IS("no_value") && is_v7(name))
		return 0;
	n = real_confstr(name, buf, len);
	if (IS("errno_on_success") && n != 0)
		errno = ENOENT;
	return n;
}
