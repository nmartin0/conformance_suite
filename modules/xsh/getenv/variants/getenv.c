/*
 * getenv.c - conforming variants of getenv(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library. It replaces getenv() itself, so it reads PC_VARIANT
 * from the environment once, at load, instead of through pcvariant.h's
 * variant_of() (which calls getenv()).
 */
#define _GNU_SOURCE 1
#include <stdlib.h>
#include <string.h>

extern char **environ;

static char variant[64];

static const char *raw_env(const char *name)
{
	size_t n = strlen(name), i;

	for (i = 0; environ != NULL && environ[i] != NULL; i++)
		if (strncmp(environ[i], name, n) == 0 && environ[i][n] == '=')
			return environ[i] + n + 1;
	return NULL;
}

__attribute__((constructor)) static void load(void)
{
	const char *v = raw_env("PC_VARIANT");

	if (v != NULL && strncmp(v, "getenv.", 7) == 0 && strlen(v + 7) < sizeof variant)
		strcpy(variant, v + 7);
}

/*
 * "copies_on_reassign": when getenv() finds that environ points to an
 * array it has not seen, it copies the array and its strings into storage
 * of its own and points environ there before searching. XSH getenv()
 * RATIONALE describes exactly this reinitialisation after the application
 * assigns environ; the normative text asks only that getenv() search the
 * environment and return a pointer into the data environ points to, which
 * the copy still is.
 */
static char **copy_environ(void)
{
	size_t n, i;
	char **a;

	for (n = 0; environ[n] != NULL; n++)
		;
	a = malloc((n + 1) * sizeof *a);
	if (a == NULL)
		return NULL;
	for (i = 0; i < n; i++)
		if ((a[i] = strdup(environ[i])) == NULL)
			return NULL;
	a[n] = NULL;
	return a;
}

char *getenv(const char *name)
{
	static char **seen;
	size_t n = strlen(name), i;

	if (strcmp(variant, "copies_on_reassign") == 0 && environ != NULL && environ != seen) {
		char **a = copy_environ();

		if (a != NULL)
			environ = a;
		seen = environ;
	}
	for (i = 0; environ != NULL && environ[i] != NULL; i++)
		if (strncmp(environ[i], name, n) == 0 && environ[i][n] == '=')
			return environ[i] + n + 1;
	return NULL;
}
