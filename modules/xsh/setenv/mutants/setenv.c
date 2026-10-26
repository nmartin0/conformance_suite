/*
 * setenv.c - planted faults in setenv(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "setenv.<fault>" and apply only
 * while PC_PHASE is "test", and never to pc.c's own setenv("PC_PHASE")
 * calls, which also tell this library the phase.
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

static int (*real_setenv(void))(const char *, const char *, int)
{
	static int (*fn)(const char *, const char *, int);

	if (fn == NULL)
		fn = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	return fn;
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

static char *entry(const char *name, const char *value)
{
	char *s = malloc(strlen(name) + strlen(value) + 2);

	if (s != NULL) {
		strcpy(s, name);
		strcat(s, "=");
		strcat(s, value);
	}
	return s;
}

/* "keeps_value_pointer": getenv() hands back the caller's value array. */
static char *kept_name;
static const char *kept_value;

char *getenv(const char *name)
{
	static char *(*real)(const char *);

	if (real == NULL)
		real = (char *(*)(const char *))dlsym(RTLD_NEXT, "getenv");
	if (kept_name != NULL && strcmp(name, kept_name) == 0)
		return (char *)kept_value;
	return real(name);
}

int setenv(const char *name, const char *value, int overwrite)
{
	const char *m;
	size_t n, i;
	char **a;
	char *s;

	if (strcmp(name, "PC_PHASE") == 0) {
		if (strlen(value) < sizeof phase)
			strcpy(phase, value);
		return real_setenv()(name, value, overwrite);
	}
	m = mutant_of("setenv.");
	if (m == NULL)
		return real_setenv()(name, value, overwrite);
	if (IS("not_added") && !exists(name))
		return 0;
	if (IS("no_overwrite_means_no_add") && overwrite == 0)
		return 0;
	if (IS("ignores_overwrite"))
		overwrite = 1;
	if (IS("overwrite_one_only") && overwrite != 1)
		overwrite = 0;
	if (IS("no_overwrite_fails") && overwrite == 0 && exists(name))
		return fail(EEXIST);
	if (IS("accepts_empty") && *name == '\0')
		return 0;
	if ((IS("cuts_at_equals") || IS("adds_then_fails")) && strchr(name, '=') != NULL) {
		char cut[256];

		n = (size_t)(strchr(name, '=') - name);
		if (n == 0 || n >= sizeof cut)
			return real_setenv()(name, value, overwrite);
		memcpy(cut, name, n);
		cut[n] = '\0';
		if (real_setenv()(cut, value, overwrite) != 0)
			return -1;
		return IS("adds_then_fails") ? fail(EINVAL) : 0;
	}
	if (IS("duplicates") && overwrite != 0 && exists(name)) {
		/* appends a second string instead of replacing the first */
		for (n = 0; environ[n] != NULL; n++)
			;
		a = malloc((n + 2) * sizeof *a);
		s = entry(name, value);
		if (a == NULL || s == NULL)
			return fail(ENOMEM);
		for (i = 0; i < n; i++)
			a[i] = environ[i];
		a[n] = s;
		a[n + 1] = NULL;
		environ = a;
		return 0;
	}
	if (IS("drops_entry") && !exists(name)) {
		/* the new string lands on the last slot instead of after it */
		for (n = 0; environ[n] != NULL; n++)
			;
		s = entry(name, value);
		if (s == NULL)
			return fail(ENOMEM);
		for (i = n; i-- > 0;)
			if (strncmp(environ[i], "PC_", 3) != 0) {
				environ[i] = s;
				return 0;
			}
		return real_setenv()(name, value, overwrite);
	}
	if (IS("appends_in_place") && !exists(name)) {
		for (n = 0; environ[n] != NULL; n++)
			;
		s = entry(name, value);
		if (s == NULL)
			return fail(ENOMEM);
		environ[n] = s;		/* writes the slot after the terminator too */
		environ[n + 1] = NULL;
		return 0;
	}
	if (IS("own_copy") && environ != initial && initial != NULL)
		environ = initial;	/* goes back to its own array, dropping the application's */
	if (IS("keeps_value_pointer")) {
		if (real_setenv()(name, value, overwrite) != 0)
			return -1;
		free(kept_name);
		kept_name = strdup(name);
		kept_value = value;
		return 0;
	}
	return real_setenv()(name, value, overwrite);
}
