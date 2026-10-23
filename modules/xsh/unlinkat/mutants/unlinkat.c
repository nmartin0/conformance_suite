/*
 * unlinkat.c - planted faults in unlinkat(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "<function>.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
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

int unlinkat(int fd, const char *path, int flag)
{
	REAL(unlinkat);
	const char *m = mutant_of("unlinkat.");
	struct stat st;
	int r;

	if (m == NULL)
		return real_unlinkat(fd, path, flag);
	if (IS("ignores_fd") && path[0] != '/')
		fd = AT_FDCWD;
	if (IS("removedir_ignored"))
		flag &= ~AT_REMOVEDIR;
	if (IS("removedir_noop") && (flag & AT_REMOVEDIR))
		return 0;
	if (IS("file_as_dir") && (flag & AT_REMOVEDIR) && fstatat(fd, path, &st, AT_SYMLINK_NOFOLLOW) == 0 &&
	    !S_ISDIR(st.st_mode))
		flag &= ~AT_REMOVEDIR;
	if (IS("missing_ok") && fstatat(fd, path, &st, AT_SYMLINK_NOFOLLOW) != 0)
		return 0;
	if ((IS("bad_fd_as_cwd") || IS("file_fd_as_cwd")) && fd != AT_FDCWD && path[0] != '/' &&
	    (fstat(fd, &st) != 0 || !S_ISDIR(st.st_mode)))
		fd = AT_FDCWD;
	r = real_unlinkat(fd, path, flag);
	if (r == -1 && IS("nonempty_ok") && (errno == EEXIST || errno == ENOTEMPTY))
		return 0;
	if (r == -1 && IS("nonempty_einval") && (errno == EEXIST || errno == ENOTEMPTY))
		errno = EINVAL;
	if (r == -1 && IS("dir_eacces") && (errno == EPERM || errno == EISDIR))
		errno = EACCES;
	if (r == -1 && IS("eacces_as_enoent") && errno == EACCES)
		errno = ENOENT;
	return r;
}
