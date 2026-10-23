/*
 * rmdir.c - planted faults in rmdir(), for mutation checks (CI only).
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

int rmdir(const char *path)
{
	REAL(rmdir);
	const char *m = mutant_of("rmdir.");
	char dbuf[4096], target[4096], b[4096];
	struct stat st;
	ssize_t n;
	int r;

	if (m == NULL)
		return real_rmdir(path);
	if (IS("noop") || (IS("missing_ok") && lstat(path, &st) != 0))
		return 0;
	if (IS("follows_link") && lstat(path, &st) == 0 && S_ISLNK(st.st_mode) &&
	    (n = readlink(path, target, sizeof target - 1)) > 0) {
		target[n] = '\0';
		return real_rmdir(target);
	}
	if (IS("removes_files") && lstat(path, &st) == 0 && S_ISREG(st.st_mode))
		return unlink(path);
	if (IS("dot_ok") && last_is(path, "."))
		return 0;
	if (IS("slash_rejected") && has_slash_end(path))
		return fail(ENOTDIR);
	if (IS("slash_ignored") && has_slash_end(path) && strlen(path) < sizeof b) {
		strcpy(b, path);
		while (strlen(b) > 1 && b[strlen(b) - 1] == '/')
			b[strlen(b) - 1] = '\0';
		if (lstat(b, &st) == 0 && !S_ISDIR(st.st_mode))
			return unlink(b);
	}
	if (IS("nonempty_removes") && stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
		/* empties a directory of plain files first */
		DIR *d = opendir(path);
		struct dirent *e;
		while (d && (e = readdir(d)) != NULL) {
			if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
				continue;
			snprintf(b, sizeof b, "%s/%s", path, e->d_name);
			(void)unlink(b);
		}
		if (d)
			(void)closedir(d);
	}
	if (IS("sticky_refuses_owner")) {
		struct stat d;
		if (stat(dir_of(path, dbuf, sizeof dbuf), &d) == 0 && (d.st_mode & S_ISVTX) && d.st_uid != geteuid())
			return fail(EPERM);
	}
	if (IS("times_kept")) {
		const char *d = dir_of(path, dbuf, sizeof dbuf);
		struct stat before;
		if (stat(d, &before) == 0) {
			r = real_rmdir(path);
			keep_mtime(d, &before);
			return r;
		}
	}
	r = real_rmdir(path);
	if (r == -1 && IS("nonempty_ok") && (errno == EEXIST || errno == ENOTEMPTY))
		return 0;
	if (r == -1 && IS("nonempty_einval") && (errno == EEXIST || errno == ENOTEMPTY))
		errno = EINVAL;
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
	if (r == -1 && IS("sticky_ignored") && (errno == EPERM || errno == EACCES))
		return 0;
	if (r == 0 && IS("returns_one"))
		return 1;
	return r;
}
