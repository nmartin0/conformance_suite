/*
 * fileno.c - conforming variants of fileno(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "checks_fd": fileno() checks that the stream's descriptor is open and
 * fails with EBADF when it is not. XSH fileno() lists that as a "may fail"
 * error.
 */
int fileno(FILE *f)
{
	const char *v = variant_of("fileno.");
	int r;
	REAL(fileno);

	r = real_fileno(f);
	if (IS("checks_fd") && r >= 0 && fcntl(r, F_GETFD) == -1) {
		errno = EBADF;
		return -1;
	}
	return r;
}
