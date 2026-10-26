/*
 * chdir.c - conforming variants of chdir(), for the variant checks (CI
 * only).
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
 * "enoent_for_file" (a READING of XSH.chdir.004): chdir() reports ENOENT
 * wherever a component is not a directory. XSH chdir() ERRORS words ENOENT
 * as a component that "does not name an existing directory", which a
 * regular file meets, and XSH 2.3 lets either of two applicable errors be
 * reported.
 */
int chdir(const char *path)
{
	const char *v = variant_of("chdir.");
	int r;
	REAL(chdir);

	r = real_chdir(path);
	if (r == -1 && IS("enoent_for_file") && errno == ENOTDIR)
		errno = ENOENT;
	return r;
}
