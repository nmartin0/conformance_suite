/*
 * symlink.c - planted faults in symlink() and symlinkat(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "symlink.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 * The "at_" faults apply only to symlinkat().
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

static void cannot_plant(const char *what)
{
	fprintf(stderr, "pcmutant: cannot plant (%s: %s)\n", what, strerror(errno));
}

/* Copy of p without its trailing slashes (a lone "/" is kept). */
static const char *stripped(const char *p, char *buf, size_t size)
{
	size_t n = strlen(p);

	while (n > 1 && p[n - 1] == '/')
		n--;
	if (n >= size)
		n = size - 1;
	memcpy(buf, p, n);
	buf[n] = '\0';
	return buf;
}

/* The directory part of p ("." if none) into buf. */
static const char *dir_of(const char *p, char *buf, size_t size)
{
	const char *s = strrchr(p, '/');
	size_t n = s ? (size_t)(s - p) : 0;

	if (s == NULL)
		return ".";
	if (n == 0)
		return "/";
	if (n >= size)
		n = size - 1;
	memcpy(buf, p, n);
	buf[n] = '\0';
	return buf;
}

/* The last component of p. */
static const char *last_of(const char *p)
{
	const char *s = strrchr(p, '/');

	return s ? s + 1 : p;
}

static const char *cur_target;	/* path1 of the call being made */

static int real_make(int fd, const char *path)
{
	REAL(symlinkat);
	return real_symlinkat(cur_target, fd, path);
}

static int make(int at, int fd, const char *path)
{
	const char *m = mutant_of("symlink.");
	char b[4096], t[4096];
	struct stat st;
	ssize_t n;
	int r;

	if (m == NULL)
		return real_make(fd, path);
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
	if (IS("noop"))
		return 0;
	if (IS("empty_ok") && path[0] == '\0')
		return 0;
	if (IS("eexist_ok") && fstatat(fd, path, &st, AT_SYMLINK_NOFOLLOW) == 0)
		return 0;
	if (IS("replaces")) {
		(void)unlinkat(fd, path, 0);
		(void)unlinkat(fd, path, AT_REMOVEDIR);
	}
	if (IS("follows_link") && fstatat(fd, path, &st, AT_SYMLINK_NOFOLLOW) == 0 && S_ISLNK(st.st_mode) &&
	    (n = readlinkat(fd, path, t, sizeof t - 1)) > 0) {
		t[n] = '\0';
		path = t;	/* creates the file the dangling link points at */
	}
	if (IS("makes_parents") && strchr(path, '/') != NULL)
		(void)mkdirat(fd, dir_of(path, b, sizeof b), 0755);
	if (IS("slash_stripped"))
		path = stripped(path, b, sizeof b);
	if (IS("slash_enoent_existing") && strlen(path) > 1 && path[strlen(path) - 1] == '/')
		return fail(ENOENT);
	if (IS("long_truncated") && strlen(last_of(path)) > 255 && strlen(path) < sizeof b) {
		strcpy(b, path);
		b[(size_t)(last_of(path) - path) + 255] = '\0';	/* keeps {NAME_MAX} bytes, as without _POSIX_NO_TRUNC */
		path = b;
	}
	if (IS("eilseq_as_einval") && strchr(last_of(path), '\n') != NULL)
		return fail(EINVAL);
	if (IS("times_kept")) {
		struct stat before;
		const char *d = dir_of(path, t, sizeof t);
		if (fstatat(fd, d, &before, 0) == 0) {
			struct timespec ts[2];
			r = real_make(fd, path);
			ts[0] = before.st_atim;
			ts[1] = before.st_mtim;
			(void)utimensat(fd, d, ts, 0);
			return r;
		}
	}
	if (IS("makes_file")) {
		int f = openat(fd, path, O_WRONLY | O_CREAT | O_EXCL, 0644);
		if (f < 0)
			return -1;
		(void)write(f, cur_target, strlen(cur_target));
		return close(f);
	}
	if (IS("truncates") && strlen(cur_target) > 1 && strlen(cur_target) < sizeof t) {
		strcpy(t, cur_target);
		t[strlen(t) - 1] = '\0';	/* loses the last byte of path1 */
		cur_target = t;
	}
	if (IS("validates") && fstatat(fd, cur_target, &st, 0) != 0)
		return fail(ENOENT);	/* checks path1 as a pathname */
	if (IS("normalizes") && strlen(cur_target) < sizeof t) {
		const char *s;
		char *o = t;
		for (s = cur_target; *s != '\0'; s++)
			if (!(*s == '/' && o > t && o[-1] == '/'))
				*o++ = *s;
		while (o > t + 1 && o[-1] == '/')
			o--;
		*o = '\0';
		cur_target = t;
	}
	if (IS("empty_enoent") && cur_target[0] == '\0')
		return fail(ENOENT);
	r = real_make(fd, path);
	if (r == 0 && IS("returns_one"))
		return 1;
	if (r == 0 && IS("old_times")) {
		struct timespec ts[2] = { { 1, 0 }, { 1, 0 } };
		if (utimensat(fd, path, ts, AT_SYMLINK_NOFOLLOW) != 0)
			cannot_plant("utimensat");
	}
	if (r == 0 && IS("wrong_owner") && fchownat(fd, path, 4243, (gid_t)-1, AT_SYMLINK_NOFOLLOW) != 0)
		cannot_plant("fchownat");
	if (r == 0 && IS("wrong_group") && fchownat(fd, path, (uid_t)-1, 4244, AT_SYMLINK_NOFOLLOW) != 0)
		cannot_plant("fchownat");
	if (r == -1 && IS("enotdir_as_enoent") && errno == ENOTDIR)
		errno = ENOENT;
	if (r == -1 && IS("eloop_as_enoent") && errno == ELOOP)
		errno = ENOENT;
	if (r == -1 && IS("eacces_as_eperm") && errno == EACCES)
		errno = EPERM;
	if (r == -1 && at && IS("at_eacces_as_enoent") && errno == EACCES)
		errno = ENOENT;
	return r;
}

int symlink(const char *path1, const char *path2)
{
	cur_target = path1;
	return make(0, AT_FDCWD, path2);
}

int symlinkat(const char *path1, int fd, const char *path2)
{
	cur_target = path1;
	return make(1, fd, path2);
}

/*
 * "readlink_mode": readlink() checks the link's owner as if its contents
 * needed permission to read.
 */
ssize_t readlink(const char *restrict path, char *restrict buf, size_t size)
{
	const char *m = mutant_of("symlink.");
	struct stat st;
	REAL(readlink);

	if (IS("readlink_mode") && lstat(path, &st) == 0 && S_ISLNK(st.st_mode) && st.st_uid != geteuid())
		return fail(EACCES);
	return real_readlink(path, buf, size);
}

/*
 * "max_misreported": pathconf() reports a {SYMLINK_MAX} of 64 that
 * symlink() does not enforce. It is read in the setup phase, so it applies
 * whatever the phase.
 */
long pathconf(const char *path, int name)
{
	const char *m = getenv("PC_MUTANT");
	REAL(pathconf);

	if (m != NULL && strcmp(m, "symlink.max_misreported") == 0 && name == _PC_SYMLINK_MAX)
		return 64;
	return real_pathconf(path, name);
}
