/*
 * fchmodat.c - planted faults in fchmodat(), for mutation checks (CI only).
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


int fchmodat(int fd, const char *path, mode_t mode, int flag)
{
	REAL(fchmodat);
	const char *m = mutant_of("fchmodat.");
	struct stat st;
	int r;

	if (m == NULL)
		return real_fchmodat(fd, path, mode, flag);
	if (IS("ignores_fd") && path[0] != '/')
		fd = AT_FDCWD;
	if (IS("fdcwd_ebadf") && fd == AT_FDCWD)
		return fail(EBADF);
	if (IS("checks_fd") && fd != AT_FDCWD && fstat(fd, &st) != 0)
		return fail(EBADF);
	if (IS("missing_ok") && fstatat(fd, path, &st, 0) != 0)
		return 0;
	if (IS("ignores_nofollow"))
		flag &= ~AT_SYMLINK_NOFOLLOW;
	if (IS("nofollow_fails_on_files") && (flag & AT_SYMLINK_NOFOLLOW))
		return fail(EOPNOTSUPP);
	if (IS("search_checked") && fd != AT_FDCWD && path[0] != '/' && fstat(fd, &st) == 0 && S_ISDIR(st.st_mode) &&
	    !(st.st_mode & 0111))
		return fail(EACCES);
	if ((IS("bad_fd_as_cwd") || IS("file_fd_as_cwd")) && fd != AT_FDCWD && path[0] != '/') {
		int ok = fstat(fd, &st) == 0;
		if ((IS("bad_fd_as_cwd") && !ok) || (IS("file_fd_as_cwd") && ok && !S_ISDIR(st.st_mode)))
			fd = AT_FDCWD;
	}
	r = real_fchmodat(fd, path, mode, flag);
	if (r == -1 && IS("eacces_as_enoent") && errno == EACCES)
		errno = ENOENT;
	return r;
}
