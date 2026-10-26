/*
 * chdir.c - planted faults in chdir(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "chdir.<fault>" and apply only
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


/* Some prefix of path (component by component, with lstat()) is a symbolic link. */
static int has_link(const char *path)
{
	char b[4096];
	struct stat st;
	size_t n = strlen(path), i;

	if (n >= sizeof b)
		return 0;
	for (i = 1; i <= n; i++)
		if (i == n || path[i] == '/') {
			memcpy(b, path, i);
			b[i] = '\0';
			if (lstat(b, &st) == 0 && S_ISLNK(st.st_mode))
				return 1;
		}
	return 0;
}

int chdir(const char *path)
{
	const char *m = mutant_of("chdir.");
	char b[4096];
	size_t n;
	int r, e;
	REAL(chdir);

	if (m == NULL)
		return real_chdir(path);
	if (IS("noop"))
		return 0;
	if (IS("empty_ok") && path[0] == '\0')
		return 0;
	if (IS("no_follow") && has_link(path))
		return fail(ENOTDIR);	/* treats a symbolic link as a non-directory */
	n = strlen(path);
	if (IS("slash_enoent") && n > 1 && path[n - 1] == '/')
		return fail(ENOENT);
	if (IS("long_truncated") && n < sizeof b) {
		const char *s = strrchr(path, '/');
		size_t at = s ? (size_t)(s - path) + 1 : 0;
		if (n - at > 255) {
			memcpy(b, path, at + 255);	/* keeps {NAME_MAX} bytes, as without _POSIX_NO_TRUNC */
			b[at + 255] = '\0';
			path = b;
		}
	}
	r = real_chdir(path);
	e = errno;
	if (r == 0 && IS("returns_one"))
		return 1;
	if (r == -1 && IS("partial") && (e == ENOENT || e == ENOTDIR) && strrchr(path, '/') != NULL && n < sizeof b) {
		/* walks the name and stays in the last directory it reached */
		memcpy(b, path, (size_t)(strrchr(path, '/') - path));
		b[strrchr(path, '/') - path] = '\0';
		if (b[0] != '\0')
			(void)real_chdir(b);
		errno = e;
		return -1;
	}
	if (r == -1 && IS("file_ok") && e == ENOTDIR)
		return 0;	/* checks only that the name exists */
	if (r == -1 && IS("enoent_as_enotdir") && e == ENOENT)
		errno = ENOTDIR;
	if (r == -1 && IS("enotdir_as_einval") && e == ENOTDIR)
		errno = EINVAL;
	if (r == -1 && IS("eloop_as_enametoolong") && e == ELOOP)
		errno = ENAMETOOLONG;
	if (r == -1 && IS("eacces_as_eperm") && e == EACCES)
		errno = EPERM;
	return r;
}
