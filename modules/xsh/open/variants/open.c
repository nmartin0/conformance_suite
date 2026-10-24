/*
 * open.c - conforming variants of open() and openat() (and of what a
 * system that behaves so must also show through fcntl(), stat() and
 * pathconf()), for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it. This file is built without the variant headers, so
 * the O_* names below are the real system's values.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "accmode_bits": the file access modes are separate bits, none of them 0
 * (the header half is open.accmode_bits.h). XBD <fcntl.h> asks only that
 * the access modes be unique (O_EXEC and O_SEARCH may be equal) and that
 * O_ACCMODE mask them; nothing makes O_RDONLY zero, and the open() RATIONALE
 * even suggests encoding read and write as bits. open() and openat() take
 * the new values; fcntl(F_GETFL) reports them. A call made by code built
 * without the header (the harness) carries none of the new bits and is
 * passed on unchanged.
 */
#define V_RDONLY 0x10000000
#define V_WRONLY 0x20000000
#define V_RDWR (V_RDONLY | V_WRONLY)
#define V_SEARCH 0x40000000
#define V_ACCMODE (V_RDWR | V_SEARCH)

/*
 * "tty_init_bit": O_TTY_INIT is a bit of its own (open.tty_init_bit.h).
 * XBD <fcntl.h> lets it be zero or a distinct flag. This system takes the
 * bit off before the real call; for a file that is not a terminal open()
 * ignores the flag either way.
 */
#define V_TTY_INIT 0x04000000

/*
 * "name_max_min": {NAME_MAX} is _POSIX_NAME_MAX (see name_too_long()).
 * Not declared on any test: XBD <limits.h> gives {NAME_MAX} an XSI minimum
 * of {_XOPEN_NAME_MAX} (255) and the reference runs claim XSI, so 14
 * models a non-XSI system. Declare it again for a profile without XSI.
 */
#define V_NAME_MAX 14

static int from_variant_flags(const char *v, int flags)
{
	if (IS("accmode_bits") && (flags & V_ACCMODE) != 0) {
		int acc = flags & V_ACCMODE;

		flags &= ~V_ACCMODE;
		if (acc == V_RDONLY)
			return flags | O_RDONLY;
		if (acc == V_WRONLY)
			return flags | O_WRONLY;
		if (acc == V_RDWR)
			return flags | O_RDWR;
#ifdef O_SEARCH
		if (acc == V_SEARCH)
			return flags | O_SEARCH;
#endif
		return -1;	/* not one access mode: oflag is invalid (EINVAL may-fail) */
	}
	if (IS("tty_init_bit"))
		flags &= ~V_TTY_INIT;
	return flags;
}

static int to_variant_getfl(const char *v, int fd, int r)
{
	if (r == -1)
		return r;
	if (IS("accmode_bits")) {
		int acc = r & O_ACCMODE, n;

		r &= ~O_ACCMODE;
		if (acc == O_RDONLY)
			n = V_RDONLY;
		else if (acc == O_WRONLY)
			n = V_WRONLY;
		else if (acc == O_RDWR)
			n = V_RDWR;
		else
			n = V_SEARCH;
		r |= n;
	}
	/*
	 * "nonblock_hidden_regular": for a file other than a FIFO or a device
	 * that supports non-blocking opens, open() says it is unspecified
	 * whether the status flags include O_NONBLOCK; this system leaves it
	 * out for regular files.
	 */
	if (IS("nonblock_hidden_regular")) {
		struct stat st;

		if (fstat(fd, &st) == 0 && S_ISREG(st.st_mode))
			r &= ~O_NONBLOCK;
	}
	return r;
}

/*
 * "name_max_min": the file system of the run directory has {NAME_MAX} at
 * its minimum, {_POSIX_NAME_MAX} (14), which XBD <limits.h> allows for a
 * pathname variable; pathconf() says so for relative paths, and open()
 * of a relative path with a longer component fails with ENAMETOOLONG
 * (required by open() ERRORS). Absolute paths are taken to lie on other
 * file systems and are left alone.
 */
static int name_too_long(const char *v, int dirfd, const char *path)
{
	const char *p = path, *s;

	if (!IS("name_max_min") || path[0] == '/')
		return 0;
	(void)dirfd;
	while (*p != '\0') {
		s = strchr(p, '/');
		if (s == NULL)
			s = p + strlen(p);
		if (s - p > V_NAME_MAX)
			return 1;
		p = *s == '/' ? s + 1 : s;
	}
	return 0;
}

/*
 * "tslash_enotdir" and "tslash_enoent": open() ERRORS, for O_CREAT and a
 * path that ends in slashes after a non-slash: the call fails with ENOENT
 * or ENOTDIR, except that ENOENT is not allowed when the name without the
 * slashes exists. tslash_enotdir gives ENOTDIR in every case; tslash_enoent
 * gives ENOENT when the name does not exist and ENOTDIR when it names a
 * non-directory. A name that resolves to a directory is left to the real
 * call (EISDIR applies to an existing directory with O_CREAT).
 * Returns the errno to fail with, or 0.
 */
static int trailing_slash(const char *v, int dirfd, const char *path, int flags)
{
	char buf[4096];
	size_t n = strlen(path);
	struct stat st;

	if (!(IS("tslash_enotdir") || IS("tslash_enoent")) || !(flags & O_CREAT))
		return 0;
	if (n < 2 || n >= sizeof buf || path[n - 1] != '/')
		return 0;
	memcpy(buf, path, n + 1);
	while (n > 0 && buf[n - 1] == '/')
		buf[--n] = '\0';
	if (n == 0)
		return 0;	/* only slashes */
	if (fstatat(dirfd, buf, &st, AT_SYMLINK_NOFOLLOW) != 0)
		return IS("tslash_enoent") && errno == ENOENT ? ENOENT : ENOTDIR;
	if (fstatat(dirfd, buf, &st, 0) == 0 && S_ISDIR(st.st_mode))
		return 0;
	return ENOTDIR;
}

int open(const char *path, int flags, ...)
{
	const char *v = variant_of("open.");
	mode_t mode = 0;
	int f, e;
	REAL(open);

	if (flags & O_CREAT) {
		va_list ap;
		va_start(ap, flags);
		mode = (mode_t)va_arg(ap, int);
		va_end(ap);
	}
	if (v == NULL)
		return real_open(path, flags, mode);
	f = from_variant_flags(v, flags);
	if (f == -1) {
		errno = EINVAL;
		return -1;
	}
	if (name_too_long(v, AT_FDCWD, path)) {
		errno = ENAMETOOLONG;
		return -1;
	}
	if ((e = trailing_slash(v, AT_FDCWD, path, f)) != 0) {
		errno = e;
		return -1;
	}
	return real_open(path, f, mode);
}

int openat(int dirfd, const char *path, int flags, ...)
{
	const char *v = variant_of("open.");
	mode_t mode = 0;
	int f, e;
	REAL(openat);

	if (flags & O_CREAT) {
		va_list ap;
		va_start(ap, flags);
		mode = (mode_t)va_arg(ap, int);
		va_end(ap);
	}
	if (v == NULL)
		return real_openat(dirfd, path, flags, mode);
	f = from_variant_flags(v, flags);
	if (f == -1) {
		errno = EINVAL;
		return -1;
	}
	if (name_too_long(v, dirfd, path)) {
		errno = ENAMETOOLONG;
		return -1;
	}
	if ((e = trailing_slash(v, dirfd, path, f)) != 0) {
		errno = e;
		return -1;
	}
	return real_openat(dirfd, path, f, mode);
}

static int fcntl_common(int (*real)(int, int, ...), int fd, int cmd, void *arg)
{
	const char *v = variant_of("open.");

	if (v != NULL && cmd == F_GETFL)
		return to_variant_getfl(v, fd, real(fd, cmd));
	if (v != NULL && cmd == F_SETFL && IS("accmode_bits"))
		/* F_SETFL ignores the access mode bits, in either encoding. */
		arg = (void *)(intptr_t)((int)(intptr_t)arg & ~V_ACCMODE);
	return real(fd, cmd, arg);
}

int fcntl(int fd, int cmd, ...)
{
	va_list ap;
	void *arg;
	REAL(fcntl);

	va_start(ap, cmd);
	arg = va_arg(ap, void *);
	va_end(ap);
	return fcntl_common(real_fcntl, fd, cmd, arg);
}

#ifdef __GLIBC__
int fcntl64(int fd, int cmd, ...)
{
	va_list ap;
	void *arg;
	REAL(fcntl64);

	va_start(ap, cmd);
	arg = va_arg(ap, void *);
	va_end(ap);
	return fcntl_common(real_fcntl64, fd, cmd, arg);
}
#endif

long pathconf(const char *path, int name)
{
	const char *v = variant_of("open.");
	REAL(pathconf);

	if (IS("name_max_min") && name == _PC_NAME_MAX && path[0] != '/')
		return V_NAME_MAX;
	return real_pathconf(path, name);
}

long fpathconf(int fd, int name)
{
	const char *v = variant_of("open.");
	REAL(fpathconf);

	if (IS("name_max_min") && name == _PC_NAME_MAX)
		return V_NAME_MAX;
	return real_fpathconf(fd, name);
}

/*
 * "coarse_times": the file system keeps timestamps to the second, the
 * coarsest resolution XBD 4.12 allows; the stat functions show whole
 * seconds. (Setting a time stores the greatest supported value not above
 * it, so whole seconds is what such a system would hold.)
 */
static int whole_seconds(const char *v, int r, struct stat *st)
{
	if (r == 0 && IS("coarse_times")) {
		st->st_atim.tv_nsec = 0;
		st->st_mtim.tv_nsec = 0;
		st->st_ctim.tv_nsec = 0;
	}
	return r;
}

int stat(const char *restrict path, struct stat *restrict st)
{
	const char *v = variant_of("open.");
	REAL(stat);

	return whole_seconds(v, real_stat(path, st), st);
}

int lstat(const char *restrict path, struct stat *restrict st)
{
	const char *v = variant_of("open.");
	REAL(lstat);

	return whole_seconds(v, real_lstat(path, st), st);
}

int fstat(int fd, struct stat *st)
{
	const char *v = variant_of("open.");
	REAL(fstat);

	return whole_seconds(v, real_fstat(fd, st), st);
}

int fstatat(int dirfd, const char *restrict path, struct stat *restrict st, int flag)
{
	const char *v = variant_of("open.");
	REAL(fstatat);

	return whole_seconds(v, real_fstatat(dirfd, path, st, flag), st);
}

/*
 * "hidden_fds": the implementation keeps descriptors of its own open in
 * the process (close() and dup() APPLICATION USAGE mention catalogs and
 * tracing); which numbers they have is not specified. XSH 2.6 still gives
 * the lowest descriptor that is not open. This system holds one just
 * above the lowest free descriptor at start-up (leaving a gap below it)
 * and one at 300. They are close-on-exec, so each new program gets its
 * own pair.
 */
__attribute__((constructor)) static void hidden_fds(void)
{
	const char *v = variant_of("open.");
	int n, low;
	REAL(open);

	if (!IS("hidden_fds"))
		return;
	n = real_open("/dev/null", O_RDONLY | O_CLOEXEC);
	if (n < 0)
		return;
	low = n;	/* the lowest free descriptor before this call */
	(void)fcntl(n, F_DUPFD_CLOEXEC, low + 1);
	(void)fcntl(n, F_DUPFD_CLOEXEC, 300);
	(void)close(n);
}
