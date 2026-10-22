/*
 * open.c - planted faults in open(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Built as a preload library. It interposes open() and, only while
 * PC_PHASE is "test", applies the fault named by PC_MUTANT. Each test
 * lists the mutants it must catch; ci/mutate.sh checks that every one
 * turns the test's PASS into FAIL.
 *
 * This is harness code, not a test: it uses dlsym(RTLD_NEXT).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

typedef int (*open_fn)(const char *, int, ...);

static open_fn real_open(void)
{
	static open_fn fn;
	if (fn == NULL)
		fn = (open_fn)dlsym(RTLD_NEXT, "open");
	return fn;
}

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");
	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL)
		return NULL;
	if (strncmp(m, "open.", 5) != 0)
		return NULL;
	return m + 5;
}

int open(const char *path, int flags, ...)
{
	mode_t mode = 0;
	const char *m;
	struct stat st;
	int fd, e;

	if (flags & O_CREAT) {
		va_list ap;
		va_start(ap, flags);
		mode = (mode_t)va_arg(ap, int);
		va_end(ap);
	}
	m = mutant();
	if (m == NULL)
		return real_open()(path, flags, mode);

#define IS(name) (strcmp(m, name) == 0)
	/* Faults applied to the arguments. */
	if (IS("creat_ignored"))
		flags &= ~O_CREAT;
	if (IS("excl_ignored"))
		flags &= ~O_EXCL;
	if (IS("trunc_ignored"))
		flags &= ~O_TRUNC;
	if (IS("append_ignored"))
		flags &= ~O_APPEND;
	if (IS("cloexec_ignored"))
		flags &= ~O_CLOEXEC;
	if (IS("cloexec_forced"))
		flags |= O_CLOEXEC;
	if (IS("directory_ignored"))
		flags &= ~O_DIRECTORY;
	if (IS("nofollow_ignored"))
		flags &= ~O_NOFOLLOW;
#ifdef O_CLOFORK
	if (IS("clofork_ignored"))
		flags &= ~O_CLOFORK;
	if (IS("clofork_forced"))
		flags |= O_CLOFORK;
#endif
	if (IS("always_creat")) {
		flags |= O_CREAT;
		mode = 0644;
	}
	if (IS("mode_ignored") && (flags & O_CREAT))
		mode = 0666;
	if (IS("empty_is_dot") && path[0] == '\0')
		path = ".";
	if (IS("excl_follows_symlink") && (flags & O_CREAT) &&
	    (flags & O_EXCL) && lstat(path, &st) == 0 && S_ISLNK(st.st_mode))
		flags &= ~O_EXCL;
	if (IS("excl_truncates") && (flags & O_CREAT) && (flags & O_EXCL)) {
		int t = real_open()(path, O_WRONLY | O_TRUNC);
		if (t >= 0)
			(void)close(t);
	}
	if (IS("creat_makes_dir") && (flags & O_CREAT) &&
	    lstat(path, &st) != 0) {
		if (mkdir(path, mode) != 0)
			return -1;
		return real_open()(path, O_RDONLY);
	}

	fd = real_open()(path, flags, mode);
	e = errno;

	/* Faults applied to the outcome. */
	if (fd < 0) {
		if (IS("eexist_as_eacces") && e == EEXIST)
			e = EACCES;
		if (IS("enoent_as_eacces") && e == ENOENT)
			e = EACCES;
		if (IS("enotdir_as_enoent") && e == ENOTDIR)
			e = ENOENT;
		if (IS("eisdir_as_eacces") && e == EISDIR)
			e = EACCES;
		if (IS("enametoolong_as_enoent") && e == ENAMETOOLONG)
			e = ENOENT;
		errno = e;
		return -1;
	}
	if (IS("after_highest")) {
		/* Hand out one above the highest open descriptor instead. */
		int hi = fd, i, n;
		for (i = fd + 1; i < 1024; i++)
			if (fcntl(i, F_GETFD) != -1)
				hi = i;
		if (hi > fd) {
			n = fcntl(fd, F_DUPFD, hi + 1);
			if (n >= 0) {
				(void)close(fd);
				fd = n;
			}
		}
	}
	if (IS("not_lowest")) {
		int n = fcntl(fd, F_DUPFD, fd + 1);
		if (n >= 0) {
			(void)close(fd);
			fd = n;
		}
	}
	if (IS("mode_ignores_umask") && (flags & O_CREAT))
		(void)fchmod(fd, mode & 07777);
	if (IS("trunc_resets_mode") && (flags & O_TRUNC))
		(void)fchmod(fd, 0644);
	if (IS("offset_at_end"))
		(void)lseek(fd, 0, SEEK_END);
#undef IS
	return fd;
}
