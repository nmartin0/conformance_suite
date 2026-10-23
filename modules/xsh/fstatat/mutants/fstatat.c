/*
 * fstatat.c - planted faults in fstatat(), for mutation checks (CI only).
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

/* Damage a successful result as fault m asks. */
static int cook(const char *m, int r, struct stat *buf)
{
	if (r != 0)
		return r;
	if (IS("size_off_by_one"))
		buf->st_size++;
	if (IS("type_dir"))
		buf->st_mode = (buf->st_mode & 07777) | S_IFDIR;
	if (IS("type_reg"))
		buf->st_mode = (buf->st_mode & 07777) | S_IFREG;
	if (IS("type_link"))
		buf->st_mode = (buf->st_mode & 07777) | S_IFLNK;
	if (IS("fifo_as_chr") && S_ISFIFO(buf->st_mode))
		buf->st_mode = (buf->st_mode & 07777) | S_IFCHR;
	if (IS("mode_bits_wrong"))
		buf->st_mode ^= 0004;
	if (IS("nlink_zero"))
		buf->st_nlink = 0;
	if (IS("nlink_one"))
		buf->st_nlink = 1;
	if (IS("ino_wrong"))
		buf->st_ino ^= 1;
	if (IS("uid_wrong"))
		buf->st_uid++;
	if (IS("gid_wrong"))
		buf->st_gid++;
	if (IS("stale_times"))
		buf->st_mtim = buf->st_ctim = buf->st_atim;
	if (IS("size_with_nul") && S_ISLNK(buf->st_mode))
		buf->st_size++;
	if (IS("size_zero") && S_ISLNK(buf->st_mode))
		buf->st_size = 0;
	if (IS("returns_one"))
		return 1;
	return 0;
}

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

int fstatat(int fd, const char *restrict path, struct stat *restrict buf, int flag)
{
	REAL(fstatat);
	const char *m = mutant_of("fstatat.");
	char tmp[4096];

	if (m == NULL)
		return real_fstatat(fd, path, buf, flag);
	if (IS("ignores_fd") && path[0] != '/')
		fd = AT_FDCWD;
	if (IS("checks_fd") && fd != AT_FDCWD && path[0] == '/') {
		struct stat s;
		if (fstat(fd, &s) != 0)
			return fail(EBADF);
		if (!S_ISDIR(s.st_mode))
			return fail(ENOTDIR);
	}
	if (IS("empty_is_fd") && path[0] == '\0' && fd != AT_FDCWD)
		return fstat(fd, buf);
	if (IS("slash_no_follow") && (flag & AT_SYMLINK_NOFOLLOW) && has_slash_end(path) && strlen(path) < sizeof tmp) {
		strcpy(tmp, path);
		tmp[strlen(tmp) - 1] = '\0';
		path = tmp;
	}
	if (IS("search_checked") && fd != AT_FDCWD && path[0] != '/') {
		struct stat s;
		if (fstat(fd, &s) == 0 && S_ISDIR(s.st_mode) && !(s.st_mode & 0111))
			return fail(EACCES);	/* checks search permission even for O_SEARCH */
	}
	if (IS("search_skipped") && fd != AT_FDCWD && path[0] != '/') {
		int r2 = real_fstatat(fd, path, buf, flag);
		if (r2 == -1 && errno == EACCES) {
			memset(buf, 0, sizeof *buf);	/* as if the search check were skipped */
			return 0;
		}
		return r2;
	}
	if (IS("abs_relative_to_fd") && path[0] == '/') {
		/* resolve the absolute path from fd as if it were relative */
		while (path[0] == '/' && path[1] != '\0')
			path++;
	}
	if (IS("ignores_nofollow"))
		flag &= ~AT_SYMLINK_NOFOLLOW;
	if (IS("always_nofollow"))
		flag |= AT_SYMLINK_NOFOLLOW;
	if (IS("fdcwd_ebadf") && fd == AT_FDCWD)
		return fail(EBADF);
	if (IS("empty_ok") && path[0] == '\0')
		path = ".";
	if (IS("missing_ok") && faccessat(fd, path, F_OK, 0) != 0 && errno == ENOENT)
		path = ".";
	if ((IS("bad_fd_as_cwd") || IS("file_fd_as_cwd")) && path[0] != '/' && fd != AT_FDCWD) {
		struct stat s;
		int ok = fstat(fd, &s) == 0;
		if ((IS("bad_fd_as_cwd") && !ok) || (IS("file_fd_as_cwd") && ok && !S_ISDIR(s.st_mode)))
			fd = AT_FDCWD;
	}
	{
		int r = real_fstatat(fd, path, buf, flag);
		if (r == -1 && IS("eacces_as_enoent") && errno == EACCES)
			errno = ENOENT;
		if (r == -1 && IS("enotdir_as_enoent") && errno == ENOTDIR)
			errno = ENOENT;
		return r;
	}
}
