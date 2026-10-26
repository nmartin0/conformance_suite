/*
 * readlink.c - planted faults in readlink() and readlinkat(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "readlink.<fault>" and apply only
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


/* The last component of p. */
static const char *last_of(const char *p)
{
	const char *s = strrchr(p, '/');

	return s ? s + 1 : p;
}

static ssize_t rl(int at, int fd, const char *path, char *buf, size_t size)
{
	const char *m = mutant_of("readlink.");
	char b[4096], t[4096];
	struct stat st, before;
	ssize_t r, k;
	REAL(readlinkat);

	if (m == NULL)
		return real_readlinkat(fd, path, buf, size);
	if (at) {
		if (IS("at_ignores_fd") && path[0] != '/')
			fd = AT_FDCWD;
		if (IS("at_fdcwd_ebadf") && fd == AT_FDCWD)
			return fail(EBADF);
		if (IS("at_checks_fd") && fd != AT_FDCWD) {
			if (fstat(fd, &st) != 0)
				return fail(EBADF);
			if (!S_ISDIR(st.st_mode))
				return fail(ENOTDIR);
		}
		if (IS("at_search_checked") && fd != AT_FDCWD && path[0] != '/' && fstat(fd, &st) == 0 &&
		    S_ISDIR(st.st_mode) && !(st.st_mode & 0111))
			return fail(EACCES);
		if ((IS("at_bad_fd_as_cwd") || IS("at_file_fd_as_cwd")) && fd != AT_FDCWD && path[0] != '/') {
			int ok = fstat(fd, &st) == 0;
			if ((IS("at_bad_fd_as_cwd") && !ok) || (IS("at_file_fd_as_cwd") && ok && !S_ISDIR(st.st_mode)))
				fd = AT_FDCWD;
		}
	}
	if (IS("dirties_buf") && size > 0)
		memset(buf, 0, size);	/* clears the buffer before it knows the call succeeds */
	if (IS("empty_einval") && path[0] == '\0')
		return fail(EINVAL);
	if (IS("slash_stripped")) {
		size_t n = strlen(path);
		while (n > 1 && path[n - 1] == '/')
			n--;
		if (n < sizeof b) {
			memcpy(b, path, n);
			b[n] = '\0';
			path = b;
		}
	}
	if (IS("erange_short")) {
		k = real_readlinkat(fd, path, t, sizeof t);
		if (k >= 0 && (size_t)k >= size)
			return fail(ERANGE);	/* refuses to truncate */
	}
	if (IS("atime_kept") && fstatat(fd, path, &before, AT_SYMLINK_NOFOLLOW) == 0) {
		struct timespec ts[2];
		r = real_readlinkat(fd, path, buf, size);
		ts[0] = before.st_atim;
		ts[1] = before.st_mtim;
		(void)utimensat(fd, path, ts, AT_SYMLINK_NOFOLLOW);
		return r;
	}
	r = real_readlinkat(fd, path, buf, size);
	if (r > 0 && IS("follows") && (size_t)r < sizeof t) {
		/* reads one link further, as if the last component were followed */
		memcpy(t, buf, (size_t)r);
		t[r] = '\0';
		if (fstatat(fd, t, &st, AT_SYMLINK_NOFOLLOW) == 0 && S_ISLNK(st.st_mode))
			r = real_readlinkat(fd, t, buf, size);
	}
	if (r >= 0 && IS("counts_nul") && (size_t)r < size) {
		buf[r] = '\0';
		r++;
	}
	if (r >= 0 && IS("writes_nul"))
		buf[r] = '\0';	/* one past the bytes placed, even when that is past bufsize */
	if (r > 0 && IS("returns_zero"))
		r = 0;
	if (r > 0 && IS("normalizes")) {
		ssize_t i, o = 0;
		for (i = 0; i < r; i++)
			if (!(buf[i] == '/' && o > 0 && buf[o - 1] == '/'))
				buf[o++] = buf[i];
		r = o;
	}
	if (r == -1 && IS("file_contents") && errno == EINVAL && fstatat(fd, path, &st, 0) == 0 && S_ISREG(st.st_mode))
		return 0;	/* a regular file reads as an empty link */
	if (r == -1 && IS("einval_as_enoent") && errno == EINVAL)
		errno = ENOENT;
	if (r == -1 && IS("enotdir_as_einval") && errno == ENOTDIR)
		errno = EINVAL;
	if (r == -1 && IS("eloop_as_einval") && errno == ELOOP)
		errno = EINVAL;
	if (r == -1 && IS("long_as_einval") && errno == ENAMETOOLONG)
		errno = EINVAL;
	if (r == -1 && IS("eacces_as_eperm") && errno == EACCES)
		errno = EPERM;
	if (r == -1 && at && IS("at_eacces_as_enoent") && errno == EACCES)
		errno = ENOENT;
	(void)last_of;
	return r;
}

ssize_t readlink(const char *restrict path, char *restrict buf, size_t size)
{
	return rl(0, AT_FDCWD, path, buf, size);
}

ssize_t readlinkat(int fd, const char *restrict path, char *restrict buf, size_t size)
{
	return rl(1, fd, path, buf, size);
}
