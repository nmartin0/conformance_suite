/*
 * getcwd.c - planted faults in getcwd() (and chdir(), for the logical fault), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "getcwd.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static char phase[16];

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
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
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

static int fail(int e)
{
	errno = e;
	return -1;
}


/* The logical path of "logical": built from chdir() arguments, unresolved. */
static char logical[8192];

/* Fails when a directory at or above the working directory is unreadable. */
static int unreadable_above(const char *cwd)
{
	char b[8192];
	size_t n = strlen(cwd), i;

	if (n >= sizeof b)
		return 0;
	memcpy(b, cwd, n + 1);
	if (access(b, R_OK) != 0)
		return 1;
	for (i = n; i > 1; i--)
		if (b[i - 1] == '/') {
			b[i - 1] = '\0';
			if (access(b, R_OK) != 0)
				return 1;
		}
	return 0;
}

char *getcwd(char *buf, size_t size)
{
	const char *m = mutant_of("getcwd.");
	static char cache[8192], copy[8192];
	char t[8192];
	char *r;
	size_t n;
	REAL(getcwd);

	if (m == NULL)
		return real_getcwd(buf, size);
	if (IS("size0_erange") && size == 0 && buf != NULL) {
		errno = ERANGE;
		return NULL;
	}
	if (IS("stale") && cache[0] != '\0') {
		if (strlen(cache) + 1 > size) {
			errno = ERANGE;
			return NULL;
		}
		strcpy(buf, cache);	/* remembers the first answer */
		return buf;
	}
	if (IS("logical") && logical[0] != '\0') {
		if (strlen(logical) + 1 > size) {
			errno = ERANGE;
			return NULL;
		}
		strcpy(buf, logical);
		return buf;
	}
	if (IS("truncates") || IS("off_by_one") || IS("trailing_slash") || IS("double_slash")) {
		r = real_getcwd(t, sizeof t);
		if (r == NULL)
			return NULL;
		if (IS("trailing_slash") && strcmp(t, "/") != 0)
			strcat(t, "/");
		if (IS("double_slash") && t[1] != '/') {
			memmove(t + 1, t, strlen(t) + 1);
			t[0] = '/';
		}
		n = strlen(t);
		if (IS("off_by_one") && n + 2 > size) {
			errno = ERANGE;
			return NULL;
		}
		if (size == 0) {
			errno = EINVAL;
			return NULL;
		}
		if (n + 1 > size) {
			if (!IS("truncates")) {
				errno = ERANGE;
				return NULL;
			}
			memcpy(buf, t, size - 1);	/* gives a truncated name instead of ERANGE */
			buf[size - 1] = '\0';
			return buf;
		}
		memcpy(buf, t, n + 1);
		return buf;
	}
	r = real_getcwd(buf, size);
	if (r != NULL && IS("stale"))
		strcpy(cache, r);
	if (r != NULL && IS("returns_copy")) {
		strcpy(copy, r);
		return copy;
	}
	if (r != NULL && IS("eperm_unreadable") && unreadable_above(r)) {
		errno = EPERM;
		return NULL;
	}
	if (r != NULL && IS("deep_enametoolong") && strlen(r) >= 4096) {
		errno = ENAMETOOLONG;
		return NULL;
	}
	return r;
}

/* "logical": chdir() keeps the name it was given, as a shell's PWD does. */
int chdir(const char *path)
{
	const char *m = mutant_of("getcwd.");
	char t[8192];
	int r;
	REAL(chdir);
	REAL(getcwd);

	if (!IS("logical"))
		return real_chdir(path);
	if (logical[0] == '\0' && real_getcwd(logical, sizeof logical) == NULL)
		logical[0] = '\0';
	r = real_chdir(path);
	if (r == 0) {
		if (path[0] == '/')
			snprintf(t, sizeof t, "%s", path);
		else
			snprintf(t, sizeof t, "%s/%s", logical, path);
		strcpy(logical, t);
	}
	return r;
}
