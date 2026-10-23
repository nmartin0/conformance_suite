/*
 * lchown.c - planted faults in lchown(), for mutation checks (CI only).
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

int lchown(const char *path, uid_t owner, gid_t group)
{
	REAL(lchown);
	const char *m = mutant_of("lchown.");
	struct stat st;
	int r;

	if (m == NULL)
		return real_lchown(path, owner, group);
	if (IS("noop") || IS("ctime_kept"))
		return 0;
	if (IS("swaps"))
		return real_lchown(path, (uid_t)group, (gid_t)owner);
	if (IS("minus_one_used") && owner == (uid_t)-1)
		owner = 0;
	if (IS("owner_ignored") && lstat(path, &st) == 0 && st.st_uid != geteuid())
		return 0;	/* reports success for a file the caller does not own */
	if (IS("restriction_ignored") && geteuid() != 0 && owner != (uid_t)-1 && lstat(path, &st) == 0 && owner != st.st_uid)
		return 0;	/* pretends a file can be given away */
	if (IS("owner_refused") && geteuid() != 0)
		return fail(EPERM);
	if (IS("missing_ok") && lstat(path, &st) != 0)
		return 0;
	if (IS("follows"))
		return chown(path, owner, group);
	r = real_lchown(path, owner, group);
	if (r == -1 && IS("eacces_as_eperm") && errno == EACCES)
		errno = EPERM;
	if (r == -1 && IS("ebadf_as_einval") && errno == EBADF)
		errno = EINVAL;
	if (r == -1 && IS("enoent_as_enotdir") && errno == ENOENT)
		errno = ENOTDIR;
	if (r == -1 && IS("enotdir_as_enoent") && errno == ENOTDIR)
		errno = ENOENT;
	if (r == -1 && IS("eloop_as_enoent") && errno == ELOOP)
		errno = ENOENT;
	if (r == -1 && IS("long_as_enoent") && errno == ENAMETOOLONG)
		errno = ENOENT;
	if (r == 0 && IS("returns_one"))
		return 1;
	return r;
}
