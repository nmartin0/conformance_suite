/*
 * mkdir.c - planted faults in mkdir(), for mutation checks (CI only).
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

static int last_is(const char *p, const char *name)
{
	const char *s = strrchr(p, '/');

	return strcmp(s ? s + 1 : p, name) == 0;
}

int mkdir(const char *path, mode_t mode)
{
	REAL(mkdir);
	const char *m = mutant_of("mkdir.");
	char b[4096], dbuf[4096], target[4096];
	struct stat st;
	ssize_t n;
	int r;

	if (m == NULL)
		return real_mkdir(path, mode);
	if (IS("noop") || (IS("empty_ok") && path[0] == '\0'))
		return 0;
	if (IS("makes_file")) {
		int fd = open(path, O_WRONLY | O_CREAT | O_EXCL, mode & 0777);
		return fd < 0 ? -1 : close(fd);
	}
	if (IS("follows_link") && lstat(path, &st) == 0 && S_ISLNK(st.st_mode) &&
	    (n = readlink(path, target, sizeof target - 1)) > 0) {
		target[n] = '\0';
		return real_mkdir(target, mode);
	}
	if (IS("eexist_ok") && lstat(path, &st) == 0)
		return 0;
	if (IS("ignores_umask")) {
		mode_t old = umask(0);
		r = real_mkdir(path, mode);
		(void)umask(old);
		return r;
	}
	if (IS("ignores_mode"))
		mode = 0777 | (mode & ~0777);
	if (IS("drops_svtx"))
		mode &= ~S_ISVTX;
	if (IS("slash_rejected") && has_slash_end(path))
		return fail(ENOENT);
	if (IS("makes_parents")) {
		const char *s = strrchr(path, '/');
		if (s && (size_t)(s - path) < sizeof b) {
			memcpy(b, path, (size_t)(s - path));
			b[s - path] = '\0';
			(void)real_mkdir(b, 0755);
		}
	}
	if (IS("times_kept")) {
		const char *d = dir_of(path, dbuf, sizeof dbuf);
		struct stat before;
		if (stat(d, &before) == 0) {
			r = real_mkdir(path, mode);
			keep_mtime(d, &before);
			return r;
		}
	}
	r = real_mkdir(path, mode);
	if (r == 0 && IS("not_empty")) {
		snprintf(b, sizeof b, "%s/.pc-extra", path);
		(void)real_mkdir(b, 0700);
	}
	if (r == 0 && IS("old_times")) {
		struct timespec ts[2] = { { 1, 0 }, { 1, 0 } };
		(void)utimensat(AT_FDCWD, path, ts, 0);
	}
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
