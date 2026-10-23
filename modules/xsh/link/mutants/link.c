/*
 * link.c - planted faults in link(), for mutation checks (CI only).
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

int link(const char *path1, const char *path2)
{
	REAL(link);
	const char *m = mutant_of("link.");
	char b1[4096], b2[4096], dbuf[4096];
	struct stat st;
	int r;

	if (m == NULL)
		return real_link(path1, path2);
	if (IS("noop") || (IS("missing_ok") && lstat(path1, &st) != 0) ||
	    (IS("empty_ok") && (path1[0] == '\0' || path2[0] == '\0')))
		return 0;
	if (IS("copies") || IS("copies_empty")) {
		/* a new file with the same contents (or none), not a link */
		int in = open(path1, O_RDONLY), out;
		ssize_t n;
		if (in < 0)
			return -1;
		out = open(path2, O_WRONLY | O_CREAT | O_EXCL, 0644);
		if (out < 0) {
			(void)close(in);
			return -1;
		}
		while (IS("copies") && (n = read(in, b1, sizeof b1)) > 0)
			(void)write(out, b1, (size_t)n);
		(void)close(in);
		(void)close(out);
		return 0;
	}
	if (IS("into_dir") && stat(path2, &st) == 0 && S_ISDIR(st.st_mode)) {
		/* links into the directory, as cp or mv would */
		const char *base = strrchr(path1, '/');
		snprintf(b2, sizeof b2, "%s/%s", path2, base ? base + 1 : path1);
		return real_link(path1, b2);
	}
	if (IS("symlink_fails") && lstat(path1, &st) == 0 && S_ISLNK(st.st_mode))
		return fail(EPERM);
	if (IS("eexist_replaces") && lstat(path2, &st) == 0)
		(void)unlink(path2);
	if (IS("slash1_ignored") && has_slash_end(path1))
		path1 = unslash(path1, b1, sizeof b1);
	if (IS("slash2_ignored") && has_slash_end(path2))
		path2 = unslash(path2, b2, sizeof b2);
	if (IS("slash2_existing_enoent") && has_slash_end(path2) &&
	    lstat(unslash(path2, b2, sizeof b2), &st) == 0)
		return fail(ENOENT);
	if (IS("dir_ok") && lstat(path1, &st) == 0 && S_ISDIR(st.st_mode))
		return 0;
	if (IS("times_kept")) {
		const char *d = dir_of(path2, dbuf, sizeof dbuf);
		struct stat before;
		if (stat(d, &before) == 0) {
			r = real_link(path1, path2);
			keep_mtime(d, &before);
			return r;
		}
	}
	r = real_link(path1, path2);
	if (r == -1 && IS("enoent_as_enotdir") && errno == ENOENT)
		errno = ENOTDIR;
	if (r == -1 && IS("enotdir_as_enoent") && errno == ENOTDIR)
		errno = ENOENT;
	if (IS("others_eperm") && stat(path1, &st) == 0 && st.st_uid != geteuid())
		return fail(EPERM);
	if (r == -1 && IS("eacces_as_eperm") && errno == EACCES)
		errno = EPERM;
	if (r == -1 && IS("eexist_as_eperm") && errno == EEXIST)
		errno = EPERM;
	if (r == -1 && IS("dir_eacces") && errno == EPERM)
		errno = EACCES;
	if (r == -1 && IS("eloop_as_enoent") && errno == ELOOP)
		errno = ENOENT;
	if (r == -1 && IS("long_as_enoent") && errno == ENAMETOOLONG)
		errno = ENOENT;
	if (r == 0 && IS("returns_one"))
		return 1;
	return r;
}
