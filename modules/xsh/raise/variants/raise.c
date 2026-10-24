/*
 * raise.c - conforming variants of raise(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <signal.h>
#include "pcvariant.h"

/*
 * "fail_positive": on failure raise() returns 1, not -1. XSH raise()
 * RETURN VALUE (aligned with ISO C) asks only for a non-zero value on
 * failure, with errno set; the error itself is unchanged.
 */
int raise(int sig)
{
	const char *v = variant_of("raise.");
	int r;
	REAL(raise);

	r = real_raise(sig);
	if (IS("fail_positive") && r != 0)
		return 1;
	return r;
}
