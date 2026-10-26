/*
 * putenv.c - planted faults in putenv(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "putenv.<fault>" and apply only
 * while PC_PHASE is "test" (followed through pc.c's setenv() calls).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

static char phase[16];

int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (strcmp(name, "PC_PHASE") == 0 && strlen(value) < sizeof phase)
		strcpy(phase, value);
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");
	size_t n = strlen(prefix);

	if (strcmp(phase, "test") != 0 || m == NULL || strncmp(m, prefix, n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

static int fail(int e)
{
	errno = e;
	return -1;
}

static int exists(const char *name, size_t n)
{
	size_t i;

	for (i = 0; environ[i] != NULL; i++)
		if (strncmp(environ[i], name, n) == 0 && environ[i][n] == '=')
			return 1;
	return 0;
}

int putenv(char *string)
{
	static int (*real)(char *);
	const char *m = mutant_of("putenv.");
	const char *eq;
	size_t n, i;
	char **a;
	int r;

	if (real == NULL)
		real = (int (*)(char *))dlsym(RTLD_NEXT, "putenv");
	if (m == NULL)
		return real(string);
	eq = strchr(string, '=');
	if (eq == NULL) {
		char name[256];

		if (strlen(string) >= sizeof name)
			return real(string);
		strcpy(name, string);
		if (IS("name_ignored"))
			return 0;
		if (IS("fails_and_removes")) {
			(void)unsetenv(name);
			return fail(EINVAL);
		}
		if (IS("wrong_errno"))
			return fail(ENOENT);
		return real(string);
	}
	n = (size_t)(eq - string);
	if (IS("copies")) {
		char *c = strdup(string);

		return c == NULL ? fail(ENOMEM) : real(c);
	}
	if (IS("keeps_old") && exists(string, n))
		return 0;
	if (IS("duplicates") && exists(string, n)) {
		/* appends the string instead of replacing the old one */
		for (i = 0; environ[i] != NULL; i++)
			;
		a = malloc((i + 2) * sizeof *a);
		if (a == NULL)
			return fail(ENOMEM);
		memcpy(a, environ, i * sizeof *a);
		a[i] = string;
		a[i + 1] = NULL;
		environ = a;
		return 0;
	}
	r = real(string);
	if (IS("returns_one") && r == 0)
		return 1;
	return r;
}
