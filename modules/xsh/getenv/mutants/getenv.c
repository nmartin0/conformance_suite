/*
 * getenv.c - planted faults in getenv() and secure_getenv(), for
 * mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "getenv.<fault>" and apply only
 * while PC_PHASE is "test". This library replaces getenv() itself, so it
 * never calls it: the fault's name is read from the environment once, at
 * load, and the phase is followed through pc.c's setenv() calls (and read
 * at load, for a process the test started with exec).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

extern char **environ;

static char phase[16];
static char fault[64];

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
	const char *p = raw_env("PC_MUTANT"), *q = raw_env("PC_PHASE");

	if (p != NULL && strncmp(p, "getenv.", 7) == 0 && strlen(p + 7) < sizeof fault)
		strcpy(fault, p + 7);
	if (q != NULL && strlen(q) < sizeof phase)
		strcpy(phase, q);
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

static const char *mutant(void)
{
	if (strcmp(phase, "test") != 0 || fault[0] == '\0')
		return NULL;
	return fault;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

/* A linear search with a planted fault m (NULL: none). */
static char *search(char **env, const char *name, const char *m)
{
	size_t n = strlen(name), i;

	for (i = 0; env != NULL && env[i] != NULL; i++) {
		if (IS("prefix_match")) {
			/* compares the name only, not that '=' follows it */
			if (strncmp(env[i], name, n) == 0)
				return env[i] + n + (env[i][n] != '\0');
			continue;
		}
		if (IS("fold_case") ? strncasecmp(env[i], name, n) == 0 : strncmp(env[i], name, n) == 0)
			if (env[i][n] == '=') {
				if (IS("empty_is_null") && env[i][n + 1] == '\0')
					return NULL;
				return env[i] + n + 1;
			}
	}
	return NULL;
}

/* "cached_table": the pointers of the environment, copied once and searched ever after. */
static char **cached(void)
{
	static char **copy;
	size_t n, i;

	if (copy == NULL) {
		for (n = 0; environ[n] != NULL; n++)
			;
		copy = malloc((n + 1) * sizeof *copy);
		if (copy == NULL)
			return environ;
		for (i = 0; i <= n; i++)
			copy[i] = environ[i];
	}
	return copy;
}

char *getenv(const char *name)
{
	const char *m = mutant();
	static char buf[4096];
	char *p;

	if (m == NULL)
		return search(environ, name, NULL);
	if (IS("cached_table"))
		return search(cached(), name, NULL);
	p = search(environ, name, m);
	if (IS("copies") && p != NULL && strlen(p) < sizeof buf) {
		strcpy(buf, p);	/* the copy ISO C allows, CX does not */
		return buf;
	}
	return p;
}

char *secure_getenv(const char *name)
{
	static char *(*real)(const char *);
	const char *m = mutant();
	static char buf[4096];
	char *p;

	if (real == NULL)
		real = (char *(*)(const char *))dlsym(RTLD_NEXT, "secure_getenv");
	if (!IS("secure_copies") && !IS("secure_empty_for_missing"))
		return real(name);
	p = real(name);
	if (IS("secure_empty_for_missing"))
		return p != NULL ? p : (char *)"";	/* "not found" as an empty value */
	if (p != NULL && strlen(p) < sizeof buf) {
		strcpy(buf, p);
		return buf;
	}
	return p;
}
