/*
 * chmod.c - planted faults in chmod(), for mutation checks (CI only).
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

static int has_slash_end(const char *p)
{
	size_t n = strlen(p);

	return n > 1 && p[n - 1] == '/';
}


int chmod(const char *path, mode_t mode)
{
	REAL(chmod);
	const char *m = mutant_of("chmod.");
	char b[4096];
	struct stat st;
	int r;

	if (m == NULL)
		return real_chmod(path, mode);
	if (IS("noop") || (IS("missing_ok") && stat(path, &st) != 0))
		return 0;
	if (IS("applies_umask")) {
		mode_t u = umask(0);
		(void)umask(u);
		mode &= ~u;
	}
	if (IS("or_bits") && stat(path, &st) == 0)
		mode |= st.st_mode & 07777;
	if (IS("drops_svtx"))
		mode &= ~S_ISVTX;
	if (IS("type_bits_einval") && (mode & S_IFMT))
		return fail(EINVAL);
	if (IS("no_follow") && lstat(path, &st) == 0 && S_ISLNK(st.st_mode))
		return 0;	/* as if the link itself were changed */
	if (IS("owner_ignored") && stat(path, &st) == 0 && st.st_uid != geteuid())
		return 0;	/* reports success without the right to change it */
	if (IS("ctime_kept"))
		return 0;	/* reports success but marks nothing for update */
	if (IS("slash_ignored") && has_slash_end(path) && strlen(path) < sizeof b) {
		strcpy(b, path);
		b[strlen(b) - 1] = '\0';
		path = b;
	}
	r = real_chmod(path, mode);
	if (r == -1 && IS("eperm_as_eacces") && errno == EPERM)
		errno = EACCES;
	if (r == -1 && IS("enoent_as_enotdir") && errno == ENOENT)
		errno = ENOTDIR;
	if (r == -1 && IS("enotdir_as_enoent") && errno == ENOTDIR)
		errno = ENOENT;
	if (r == -1 && IS("eloop_as_enoent") && errno == ELOOP)
		errno = ENOENT;
	if (r == -1 && IS("long_as_enoent") && errno == ENAMETOOLONG)
		errno = ENOENT;
	if (r == -1 && IS("eacces_as_eperm") && errno == EACCES)
		errno = EPERM;
	if (r == 0 && IS("returns_one"))
		return 1;
	return r;
}
