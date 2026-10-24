/*
 * link.c - conforming variants of link() and linkat(), for the variant
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it. A variant applies in every phase and process.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pcvariant.h"

/* Does some component of p have more than max bytes? */
static int long_component(const char *p, size_t max)
{
	size_t n = 0;

	for (; *p != '\0'; p++) {
		if (*p == '/')
			n = 0;
		else if (++n > max)
			return 1;
	}
	return 0;
}

/* p ends with a slash after at least one other character. */
static int slash_end(const char *p)
{
	size_t n = strlen(p);

	return n > 1 && p[n - 1] == '/' && strspn(p, "/") < n;
}

static int fail(int e)
{
	errno = e;
	return -1;
}

/*
 * "at_fdcwd_alt": AT_FDCWD is -3041965 (the header half is
 * link.at_fdcwd_alt.h). That value means the working directory; this
 * system's own value is then an ordinary descriptor that is not open, so
 * a relative path with it fails with EBADF (the *at() ERRORS).
 * Returns the descriptor to pass on, or AT_BAD with errno set.
 */
#define ALT_FDCWD (-3041965)
#define AT_BAD (-2147483647 - 1)
static int at_fd(const char *v, int fd, const char *path)
{
	if (!IS("at_fdcwd_alt"))
		return fd;
	if (fd == ALT_FDCWD)
		return AT_FDCWD;
	if (fd == AT_FDCWD) {
		if (path[0] == '/')
			return AT_FDCWD;	/* the descriptor is not used */
		errno = EBADF;
		return AT_BAD;
	}
	return fd;
}

/*
 * Checks a variant system makes before it links (0: go on). The text
 * lets the system report any one of the errors that apply (XSH 2.3), so
 * making these checks first is allowed.
 */
static int before(const char *v, int fd1, const char *p1, int fd2, const char *p2, int follow)
{
	struct stat st;
	int e = errno;

	/*
	 * "name_max_14": {NAME_MAX} is 14, the least _POSIX_NAME_MAX allows
	 * (XBD <limits.h>); pathconf() says so, and a longer component gives
	 * ENAMETOOLONG (link() ERRORS; XBD 4.16).
	 * Not declared on any test: XBD <limits.h> gives {NAME_MAX} an XSI minimum
	 * of {_XOPEN_NAME_MAX} (255) and the reference runs claim XSI, so 14
	 * models a non-XSI system. Declare it again for a profile without XSI.
	 */
	if (IS("name_max_14") && (long_component(p1, 14) || long_component(p2, 14)))
		return fail(ENAMETOOLONG);
	/*
	 * "access_rw": the system requires that the caller may read and write
	 * the existing file (XBD 4.7) before it links it. link() DESCRIPTION
	 * lets an implementation require access to the existing file, and
	 * ERRORS gives EACCES when it does and access is denied; which access
	 * is not said. Privileged processes pass, as access checks do. A
	 * symbolic link that is itself linked is not checked.
	 */
	if (IS("access_rw") && fstatat(fd1, p1, &st, follow ? 0 : AT_SYMLINK_NOFOLLOW) == 0 &&
	    !S_ISLNK(st.st_mode) && faccessat(fd1, p1, R_OK | W_OK, AT_EACCESS) != 0 && errno == EACCES)
		return fail(EACCES);
	/*
	 * "link_max_8": {LINK_MAX} is 8, the least _POSIX_LINK_MAX allows
	 * (XBD <limits.h>); pathconf() says so, and a link that would give a
	 * file a ninth name fails with EMLINK (link() ERRORS).
	 */
	if (IS("link_max_8") && fstatat(fd1, p1, &st, follow ? 0 : AT_SYMLINK_NOFOLLOW) == 0 &&
	    !S_ISDIR(st.st_mode) && st.st_nlink >= 8)
		return fail(EMLINK);
	(void)fd2;
	errno = e;
	return 0;
}

/* Adjust a failed link's errno as the variant system reports it. */
static int after(const char *v, int r, int fd1, const char *p1, int fd2, const char *p2, int follow)
{
	struct stat st;
	char buf[4096];
	size_t n;
	int e = errno;

	/*
	 * "slash2_enotdir": path2 ends in a slash and does not name an
	 * existing directory, path1 names an existing non-directory: report
	 * ENOTDIR. For a path2 that names nothing, link() ERRORS allows
	 * ENOENT or ENOTDIR; for one that names an existing non-directory,
	 * pathname resolution of path2 fails (XBD 4.16) and ENOENT is ruled
	 * out, and ENOTDIR is the error XBD 4.16's rule implies.
	 */
	if (r == -1 && IS("slash2_enotdir") && (e == ENOENT || e == EEXIST) && slash_end(p2) &&
	    (n = strlen(p2)) < sizeof buf &&
	    fstatat(fd1, p1, &st, follow ? 0 : AT_SYMLINK_NOFOLLOW) == 0 && !S_ISDIR(st.st_mode)) {
		memcpy(buf, p2, n + 1);
		while (n > 1 && buf[n - 1] == '/')
			buf[--n] = '\0';
		if (fstatat(fd2, buf, &st, 0) != 0 || !S_ISDIR(st.st_mode))
			return fail(ENOTDIR);
	}
	errno = e;
	return r;
}

int link(const char *path1, const char *path2)
{
	const char *v = variant_of("link.");
	int r;
	REAL(link);
	REAL(linkat);

	if (v == NULL)
		return real_link(path1, path2);
	/*
	 * "follows": link() of a symbolic link links the file it points to.
	 * link() DESCRIPTION makes following or not implementation-defined;
	 * linkat() without AT_SYMLINK_FOLLOW still links the link itself.
	 */
	if (IS("follows")) {
		if (before(v, AT_FDCWD, path1, AT_FDCWD, path2, 1) != 0)
			return -1;
		r = real_linkat(AT_FDCWD, path1, AT_FDCWD, path2, AT_SYMLINK_FOLLOW);
		return after(v, r, AT_FDCWD, path1, AT_FDCWD, path2, 1);
	}
	if (before(v, AT_FDCWD, path1, AT_FDCWD, path2, 0) != 0)
		return -1;
	r = real_link(path1, path2);
	return after(v, r, AT_FDCWD, path1, AT_FDCWD, path2, 0);
}

int linkat(int fd1, const char *path1, int fd2, const char *path2, int flag)
{
	const char *v = variant_of("link.");
	int follow = (flag & AT_SYMLINK_FOLLOW) != 0, r;
	REAL(linkat);

	if (v == NULL)
		return real_linkat(fd1, path1, fd2, path2, flag);
	if ((fd1 = at_fd(v, fd1, path1)) == AT_BAD || (fd2 = at_fd(v, fd2, path2)) == AT_BAD)
		return -1;
	if (before(v, fd1, path1, fd2, path2, follow) != 0)
		return -1;
	r = real_linkat(fd1, path1, fd2, path2, flag);
	return after(v, r, fd1, path1, fd2, path2, follow);
}

/* The limits the "name_max_14" and "link_max_8" systems report. */
static long limit(const char *v, int name, long real)
{
	if (IS("name_max_14") && name == _PC_NAME_MAX)
		return 14;
	if (IS("link_max_8") && name == _PC_LINK_MAX)
		return 8;
	return real;
}

long pathconf(const char *path, int name)
{
	const char *v = variant_of("link.");
	REAL(pathconf);

	return limit(v, name, real_pathconf(path, name));
}

long fpathconf(int fd, int name)
{
	const char *v = variant_of("link.");
	REAL(fpathconf);

	return limit(v, name, real_fpathconf(fd, name));
}

/*
 * "coarse_times": the file system keeps timestamps in whole seconds.
 * XBD 4.12 (File Times Update) makes the resolution implementation-defined
 * but no coarser than one second, and has each timestamp set to the
 * greatest supported value not after the time it is set to: truncation.
 */
static int coarse(const char *v, int r, struct stat *st)
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
	const char *v = variant_of("link.");
	REAL(stat);

	return coarse(v, real_stat(path, st), st);
}

int lstat(const char *restrict path, struct stat *restrict st)
{
	const char *v = variant_of("link.");
	REAL(lstat);

	return coarse(v, real_lstat(path, st), st);
}

int fstat(int fd, struct stat *st)
{
	const char *v = variant_of("link.");
	REAL(fstat);

	return coarse(v, real_fstat(fd, st), st);
}

int fstatat(int fd, const char *restrict path, struct stat *restrict st, int flag)
{
	const char *v = variant_of("link.");
	REAL(fstatat);

	return coarse(v, real_fstatat(fd, path, st, flag), st);
}
