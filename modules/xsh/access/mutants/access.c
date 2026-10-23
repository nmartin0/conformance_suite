/*
 * access.c - planted faults in access(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "<function>.<fault>" and apply only
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

int access(const char *path, int amode)
{
	REAL(access);
	const char *m = mutant_of("access.");
	struct stat st;
	int r;

	if (m == NULL)
		return real_access(path, amode);
	if (IS("noop") || (IS("missing_ok") && stat(path, &st) != 0))
		return 0;
	if (IS("ignores_bits"))
		amode = F_OK;
	if (IS("any_bit") && amode != F_OK) {
		/* succeeds when any one of the requested permissions is there */
		if (((amode & R_OK) && real_access(path, R_OK) == 0) || ((amode & W_OK) && real_access(path, W_OK) == 0) ||
		    ((amode & X_OK) && real_access(path, X_OK) == 0))
			return 0;
	}
	if (IS("any_class") && amode != F_OK && stat(path, &st) == 0) {
		/* lets the group and other bits stand in for the owner's */
		mode_t want = ((amode & R_OK) ? 4 : 0) | ((amode & W_OK) ? 2 : 0) | ((amode & X_OK) ? 1 : 0);
		mode_t all = ((st.st_mode >> 6) | (st.st_mode >> 3) | st.st_mode) & 7;
		return (all & want) == want ? 0 : fail(EACCES);
	}
	if (IS("privilege_ignored") && geteuid() == 0 && stat(path, &st) == 0 && (st.st_mode & 0777) == 0)
		return fail(EACCES);
	if (IS("uses_effective"))
		return faccessat(AT_FDCWD, path, amode, AT_EACCESS);
	if (IS("no_follow") && lstat(path, &st) == 0 && S_ISLNK(st.st_mode))
		return 0;
	if (IS("slash_ignored")) {
		static char b[4096];
		size_t n = strlen(path);
		if (n > 1 && n < sizeof b && path[n - 1] == '/') {
			strcpy(b, path);
			b[n - 1] = '\0';
			path = b;
		}
	}
	r = real_access(path, amode);
	if (r == -1 && IS("enoent_as_enotdir") && errno == ENOENT)
		errno = ENOTDIR;
	if (r == -1 && IS("enotdir_as_enoent") && errno == ENOTDIR)
		errno = ENOENT;
	if (r == -1 && IS("eloop_as_enoent") && errno == ELOOP)
		errno = ENOENT;
	if (r == -1 && IS("long_as_enoent") && errno == ENAMETOOLONG)
		errno = ENOENT;
	if (r == -1 && IS("eacces_as_enoent") && errno == EACCES)
		errno = ENOENT;
	return r;
}
