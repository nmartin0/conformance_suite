/*
 * setenv.c - conforming variants of setenv(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <stdlib.h>
#include <string.h>
#include "pcvariant.h"

extern char **environ;

/*
 * "front": a string setenv() adds or replaces is moved to the start of
 * the array. XBD 8.1 gives the order of the strings in the environment
 * no meaning, and setenv() only has to update the list environ points
 * to.
 * "fresh_array": every successful call leaves environ pointing to a newly
 * allocated array (the same strings). setenv() DESCRIPTION requires only
 * that the list environ points to be updated; nothing says the array
 * stays where it was (its RATIONALE warns that copies of environ may go
 * stale).
 */
int setenv(const char *name, const char *value, int overwrite)
{
	const char *v = variant_of("setenv.");
	size_t n, i, len;
	char **a, *t;
	int r;
	REAL(setenv);

	r = real_setenv(name, value, overwrite);
	if (r != 0 || v == NULL)
		return r;
	len = strlen(name);
	for (n = 0; environ[n] != NULL; n++)
		;
	if (IS("front")) {
		for (i = 0; i < n; i++)
			if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=') {
				t = environ[i];
				memmove(&environ[1], &environ[0], i * sizeof *environ);
				environ[0] = t;
				break;
			}
	} else if (IS("fresh_array")) {
		a = malloc((n + 1) * sizeof *a);
		if (a != NULL) {
			memcpy(a, environ, (n + 1) * sizeof *a);
			environ = a;
		}
	}
	return r;
}
