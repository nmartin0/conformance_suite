/*
 * getcwd.c - conforming variants of getcwd(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "eacces_unreadable": getcwd() reads every directory above the working
 * directory to find the names, and fails with EACCES when it may not read
 * one of them or may not search the working directory. XSH getcwd()
 * ERRORS lists this as a may-fail condition.
 */
static int denied(const char *cwd)
{
	char b[8192];
	size_t n = strlen(cwd), i;

	if (n >= sizeof b)
		return 0;
	memcpy(b, cwd, n + 1);
	if (access(b, X_OK) != 0)
		return 1;
	for (i = n; i > 1; i--)
		if (b[i - 1] == '/') {
			b[i - 1] = '\0';
			if (access(i > 2 ? b : "/", R_OK | X_OK) != 0)
				return 1;
		}
	return 0;
}

char *getcwd(char *buf, size_t size)
{
	const char *v = variant_of("getcwd.");
	char *r;
	REAL(getcwd);

	r = real_getcwd(buf, size);
	if (r != NULL && IS("eacces_unreadable") && denied(r)) {
		errno = EACCES;
		return NULL;
	}
	return r;
}
