/*
 * renameat.c - planted faults in renameat(), for mutation checks (CI only).
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

static const char *unslash(const char *p, char *buf, size_t size)
{
	size_t n = strlen(p);

	if (n >= size)
		return p;
	memcpy(buf, p, n + 1);
	while (n > 1 && buf[n - 1] == '/')
		buf[--n] = '\0';
	return buf;
}

static const char *dir_of(const char *p, char *buf, size_t size)
{
	const char *s = strrchr(p, '/');
	size_t n = s ? (size_t)(s - p) : 0;

	if (s == NULL || n == 0 || n >= size)
		return s == p ? "/" : ".";
	memcpy(buf, p, n);
	buf[n] = '\0';
	return buf;
}

static void keep_mtime(const char *d, const struct stat *before)
{
	struct timespec ts[2];

	ts[0] = before->st_atim;
	ts[1] = before->st_mtim;
	(void)utimensat(AT_FDCWD, d, ts, 0);
}

int renameat(int oldfd, const char *old, int newfd, const char *new)
{
	REAL(renameat);
	const char *m = mutant_of("renameat.");
	struct stat st;
	int r;

	if (m == NULL)
		return real_renameat(oldfd, old, newfd, new);
	if (IS("ignores_oldfd") && old[0] != '/')
		oldfd = AT_FDCWD;
	if (IS("ignores_newfd") && new[0] != '/')
		newfd = AT_FDCWD;
	if (IS("fdcwd_ebadf") && (oldfd == AT_FDCWD || newfd == AT_FDCWD))
		return fail(EBADF);
	if (IS("checks_fd")) {
		/* checks the descriptors even for absolute paths */
		if ((oldfd != AT_FDCWD && fstat(oldfd, &st) != 0) || (newfd != AT_FDCWD && fstat(newfd, &st) != 0))
			return fail(EBADF);
		if ((oldfd != AT_FDCWD && fstat(oldfd, &st) == 0 && !S_ISDIR(st.st_mode)) ||
		    (newfd != AT_FDCWD && fstat(newfd, &st) == 0 && !S_ISDIR(st.st_mode)))
			return fail(ENOTDIR);
	}
	if (IS("checks_both_fds") && (old[0] != '/' || new[0] != '/') &&
	    ((oldfd != AT_FDCWD && fstat(oldfd, &st) != 0) || (newfd != AT_FDCWD && fstat(newfd, &st) != 0)))
		return fail(EBADF);
	if (IS("search_checked") && oldfd != AT_FDCWD && old[0] != '/' && fstat(oldfd, &st) == 0 && S_ISDIR(st.st_mode) &&
	    !(st.st_mode & 0111))
		return fail(EACCES);
	if (IS("bad_fd_as_cwd") || IS("file_fd_as_cwd")) {
		int bad = IS("bad_fd_as_cwd");
		if (oldfd != AT_FDCWD && old[0] != '/') {
			int ok = fstat(oldfd, &st) == 0;
			if ((bad && !ok) || (!bad && ok && !S_ISDIR(st.st_mode)))
				oldfd = AT_FDCWD;
		}
		if (newfd != AT_FDCWD && new[0] != '/') {
			int ok = fstat(newfd, &st) == 0;
			if ((bad && !ok) || (!bad && ok && !S_ISDIR(st.st_mode)))
				newfd = AT_FDCWD;
		}
	}
	r = real_renameat(oldfd, old, newfd, new);
	if (r == -1 && IS("eacces_as_enoent") && errno == EACCES)
		errno = ENOENT;
	return r;
}
