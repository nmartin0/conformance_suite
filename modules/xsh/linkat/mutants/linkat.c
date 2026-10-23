/*
 * linkat.c - planted faults in linkat(), for mutation checks (CI only).
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

/* Copy p without its trailing slashes into buf. */
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

/* The directory part of p ("." if none) into buf. */
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

/* Put back the data modification time of directory d. */
static void keep_mtime(const char *d, const struct stat *before)
{
	struct timespec ts[2];

	ts[0] = before->st_atim;
	ts[1] = before->st_mtim;
	(void)utimensat(AT_FDCWD, d, ts, 0);
}

int linkat(int fd1, const char *path1, int fd2, const char *path2, int flag)
{
	REAL(linkat);
	const char *m = mutant_of("linkat.");
	struct stat st;

	if (m == NULL)
		return real_linkat(fd1, path1, fd2, path2, flag);
	if (IS("follow_ignored"))
		flag &= ~AT_SYMLINK_FOLLOW;
	if (IS("always_follow"))
		flag |= AT_SYMLINK_FOLLOW;
	if (IS("ignores_fd1") && path1[0] != '/')
		fd1 = AT_FDCWD;
	if (IS("ignores_fd2") && path2[0] != '/')
		fd2 = AT_FDCWD;
	if (IS("fdcwd_ebadf") && (fd1 == AT_FDCWD || fd2 == AT_FDCWD))
		return fail(EBADF);
	if (IS("checks_fd") && ((fd1 != AT_FDCWD && fstat(fd1, &st) != 0) || (fd2 != AT_FDCWD && fstat(fd2, &st) != 0)))
		return fail(EBADF);	/* checks fd even for an absolute path */
	if (IS("search_checked") && fd1 != AT_FDCWD && path1[0] != '/' && fstat(fd1, &st) == 0 && S_ISDIR(st.st_mode) &&
	    !(st.st_mode & 0111))
		return fail(EACCES);
	if (IS("bad_fd_as_cwd") || IS("file_fd_as_cwd")) {
		if (fd1 != AT_FDCWD && path1[0] != '/' && (fstat(fd1, &st) != 0 || !S_ISDIR(st.st_mode)))
			fd1 = AT_FDCWD;
		if (fd2 != AT_FDCWD && path2[0] != '/' && (fstat(fd2, &st) != 0 || !S_ISDIR(st.st_mode)))
			fd2 = AT_FDCWD;
	}
	{
		int r = real_linkat(fd1, path1, fd2, path2, flag);
		if (r == -1 && IS("eacces_as_enoent") && errno == EACCES)
			errno = ENOENT;
		return r;
	}
}
