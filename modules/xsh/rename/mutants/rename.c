/*
 * rename.c - planted faults in rename(), for mutation checks (CI only).
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

int rename(const char *old, const char *new)
{
	REAL(rename);
	const char *m = mutant_of("rename.");
	char b1[4096], b2[4096], d1[4096], d2[4096], target[4096];
	struct stat so, sn;
	ssize_t n;
	int r;

	if (m == NULL)
		return real_rename(old, new);
	if (IS("noop") || (IS("missing_ok") && lstat(old, &so) != 0) ||
	    (IS("empty_ok") && (old[0] == '\0' || new[0] == '\0')))
		return 0;
	if (IS("copies") && lstat(old, &so) == 0 && S_ISREG(so.st_mode)) {
		/* a new file with the same contents, then the old one removed */
		int in = open(old, O_RDONLY), out;
		if (in < 0)
			return -1;
		(void)unlink(new);
		out = open(new, O_WRONLY | O_CREAT | O_EXCL, 0644);
		while (out >= 0 && (n = read(in, b1, sizeof b1)) > 0)
			(void)write(out, b1, (size_t)n);
		(void)close(in);
		if (out >= 0)
			(void)close(out);
		return unlink(old);
	}
	if (IS("copies") && lstat(old, &so) == 0 && S_ISDIR(so.st_mode))
		return mkdir(new, 0700);	/* an empty new directory, nothing moved */
	if (IS("keeps_old") && lstat(old, &so) == 0 && !S_ISDIR(so.st_mode))
		return link(old, new);
	if (IS("keeps_new") && lstat(new, &sn) == 0)
		return unlink(old);
	if (IS("replaced_truncated") && lstat(new, &sn) == 0 && S_ISREG(sn.st_mode))
		(void)truncate(new, 0);
	if (IS("same_file_removes") && stat(old, &so) == 0 && stat(new, &sn) == 0 &&
	    so.st_dev == sn.st_dev && so.st_ino == sn.st_ino)
		return unlink(old);
	if (IS("follows_old") && lstat(old, &so) == 0 && S_ISLNK(so.st_mode) &&
	    (n = readlink(old, target, sizeof target - 1)) > 0) {
		target[n] = '\0';
		return real_rename(target, new);
	}
	if (IS("follows_new") && lstat(new, &sn) == 0 && S_ISLNK(sn.st_mode) &&
	    (n = readlink(new, target, sizeof target - 1)) > 0) {
		target[n] = '\0';
		return real_rename(old, target);
	}
	if (IS("dir_onto_dir_fails") && stat(old, &so) == 0 && S_ISDIR(so.st_mode) && stat(new, &sn) == 0 &&
	    S_ISDIR(sn.st_mode))
		return fail(EEXIST);
	if (IS("slash_ignored") && has_slash_end(new))
		new = unslash(new, b2, sizeof b2);
	if (IS("slash_old_ignored") && has_slash_end(old))
		old = unslash(old, b1, sizeof b1);
	if (IS("dir_slash_ok") && has_slash_end(new) && stat(old, &so) == 0 && S_ISDIR(so.st_mode))
		new = unslash(new, b2, sizeof b2);
	if (IS("sticky_refuses_owner")) {
		struct stat d;
		if (stat(dir_of(old, d1, sizeof d1), &d) == 0 && (d.st_mode & S_ISVTX) && d.st_uid != geteuid())
			return fail(EPERM);
	}
	if (IS("times_kept")) {
		const char *a = dir_of(old, d1, sizeof d1), *b = dir_of(new, d2, sizeof d2);
		struct stat ba, bb;
		if (stat(a, &ba) == 0 && stat(b, &bb) == 0) {
			r = real_rename(old, new);
			keep_mtime(a, &ba);
			keep_mtime(b, &bb);
			return r;
		}
	}
	r = real_rename(old, new);
	if (r == -1 && IS("nonempty_ok") && (errno == EEXIST || errno == ENOTEMPTY))
		return 0;
	if (r == -1 && IS("nonempty_einval") && (errno == EEXIST || errno == ENOTEMPTY))
		errno = EINVAL;
	if (r == -1 && IS("eisdir_as_enotdir") && errno == EISDIR)
		errno = ENOTDIR;
	if (r == -1 && IS("enotdir_as_eisdir") && errno == ENOTDIR)
		errno = EISDIR;
	if (r == -1 && IS("einval_as_enoent") && (errno == EINVAL || errno == EBUSY))
		errno = ENOENT;
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
		return 0;	/* pretends the sticky bit did not apply */
	if (r == 0 && IS("returns_one"))
		return 1;
	return r;
}
