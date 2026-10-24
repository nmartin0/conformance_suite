/*
 * ftell.c - conforming variants of ftell() and ftello(), for the variant
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include "pcvariant.h"

/*
 * "ftello_errno": a successful ftello() leaves errno changed. XSH ftell()
 * forbids that for ftell() only and says ftello() may change errno when
 * it succeeds.
 */
off_t ftello(FILE *f)
{
	const char *v = variant_of("ftell.");
	REAL(ftello);
	off_t r = real_ftello(f);

	if (IS("ftello_errno") && r != -1)
		errno = EDOM;
	return r;
}
