/*
 * unsetenv.c - conforming variants of unsetenv(), for the variant checks
 * (CI only).
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
 * "swap_last": the removed string's slot is filled with the last string
 * of the array, instead of the later strings moving down. XBD 8.1 gives
 * the order of the strings in the environment no meaning; unsetenv()
 * DESCRIPTION asks only that the variable be removed and the list environ
 * points to be updated. Invalid names still go to the real unsetenv().
 */
int unsetenv(const char *name)
{
	const char *v = variant_of("unsetenv.");
	size_t len, n, i;
	REAL(unsetenv);

	if (!IS("swap_last") || *name == '\0' || strchr(name, '=') != NULL)
		return real_unsetenv(name);
	len = strlen(name);
	for (n = 0; environ[n] != NULL; n++)
		;
	for (i = 0; i < n;)
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=') {
			environ[i] = environ[n - 1];
			environ[--n] = NULL;
		} else {
			i++;
		}
	return 0;
}
