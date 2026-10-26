/*
 * unsetenv.c - planted faults in unsetenv(), for mutation checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "unsetenv.<fault>" and apply only
 * while PC_PHASE is "test" (followed through pc.c's setenv() calls).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

static char phase[16];
static char **initial;	/* environ when the library was loaded */

__attribute__((constructor)) static void load(void)
{
	initial = environ;
}

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

static int exists(const char *name)
{
	size_t n = strlen(name), i;

	for (i = 0; environ[i] != NULL; i++)
		if (strncmp(environ[i], name, n) == 0 && environ[i][n] == '=')
			return 1;
	return 0;
}

/* Removes the last string that is not one of the harness's PC_* variables. */
static void drop_last(void)
{
	size_t n, i;

	for (n = 0; environ[n] != NULL; n++)
		;
	for (i = n; i-- > 0;)
		if (strncmp(environ[i], "PC_", 3) != 0) {
			memmove(&environ[i], &environ[i + 1], (n - i) * sizeof *environ);
			return;
		}
}

int unsetenv(const char *name)
{
	static int (*real)(const char *);
	const char *m = mutant_of("unsetenv.");
	size_t n, i;

	if (real == NULL)
		real = (int (*)(const char *))dlsym(RTLD_NEXT, "unsetenv");
	if (m == NULL)
		return real(name);
	if (IS("noop"))
		return 0;
	if (IS("accepts_empty") && *name == '\0')
		return 0;
	if ((IS("cuts_at_equals") || IS("removes_then_fails")) && strchr(name, '=') != NULL) {
		char cut[256];

		n = (size_t)(strchr(name, '=') - name);
		if (n == 0 || n >= sizeof cut)
			return real(name);
		memcpy(cut, name, n);
		cut[n] = '\0';
		(void)real(cut);
		return IS("removes_then_fails") ? fail(EINVAL) : 0;
	}
	if (IS("prefix_match") && *name != '\0' && strchr(name, '=') == NULL) {
		/* compares the name only, not that '=' follows it */
		n = strlen(name);
		for (i = 0; environ[i] != NULL;)
			if (strncmp(environ[i], name, n) == 0) {
				size_t j = i;

				while ((environ[j] = environ[j + 1]) != NULL)
					j++;
			} else {
				i++;
			}
		return 0;
	}
	if (IS("absent_fails") && *name != '\0' && strchr(name, '=') == NULL && !exists(name))
		return fail(ENOENT);
	if (IS("absent_drops_last") && *name != '\0' && strchr(name, '=') == NULL && !exists(name)) {
		drop_last();
		return 0;
	}
	if (IS("drops_last") && exists(name)) {
		if (real(name) != 0)
			return -1;
		drop_last();
		return 0;
	}
	if (IS("own_copy") && environ != initial && initial != NULL)
		environ = initial;	/* goes back to its own array, dropping the application's */
	return real(name);
}
