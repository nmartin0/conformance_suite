/*
 * access.c - conforming variants of access() and faccessat(), for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "consts_alt": F_OK is 8, R_OK 1, W_OK 2, X_OK 4, AT_FDCWD -2 and
 * AT_EACCESS 0x4000 (the header half is access.consts_alt.h). XBD
 * <unistd.h> asks only that F_OK, R_OK, W_OK, X_OK and the ORs of the
 * permission bits be distinct, so F_OK need not be 0; XBD <fcntl.h> leaves
 * AT_FDCWD (any value that is no descriptor) and AT_EACCESS to the
 * implementation. This system's own values are then ordinary: an amode
 * with unknown bits, or F_OK combined with permission bits, is invalid
 * (EINVAL, a "may fail" error of access()); amode 0 is the empty set of
 * permissions and checks only that the file can be reached; the old
 * AT_FDCWD is a descriptor that is not open (EBADF for a relative path,
 * ignored for an absolute one); the old AT_EACCESS is an unknown flag bit
 * (EINVAL, "may fail" for faccessat()).
 */
#define ALT_F 8
#define ALT_R 1
#define ALT_W 2
#define ALT_X 4
#define ALT_FDCWD (-2)
#define ALT_EACCESS 0x4000

static int alt_amode(int amode, int *out)
{
	if (amode == ALT_F || amode == 0) {
		*out = F_OK;
		return 0;
	}
	if (amode & ~(ALT_R | ALT_W | ALT_X)) {
		errno = EINVAL;
		return -1;
	}
	*out = ((amode & ALT_R) ? R_OK : 0) | ((amode & ALT_W) ? W_OK : 0) | ((amode & ALT_X) ? X_OK : 0);
	return 0;
}

/*
 * "priv_x_ok": with appropriate privileges, X_OK succeeds for a file that
 * no one may execute. access.html (DESCRIPTION) lets an implementation
 * report success for X_OK in that case, against XBD 4.7. Here the
 * privilege is that of user ID 0 (real, or effective with AT_EACCESS).
 */
static int priv_x(int amode, int eff)
{
	if (!(amode & X_OK))
		return amode;
	if ((eff ? geteuid() : getuid()) != 0)
		return amode;
	amode &= ~X_OK;
	return amode == 0 ? F_OK : amode;
}

int access(const char *path, int amode)
{
	const char *v = variant_of("access.");
	int m;
	REAL(access);

	if (IS("consts_alt")) {
		if (alt_amode(amode, &m) != 0)
			return -1;
		return real_access(path, m);
	}
	if (IS("priv_x_ok"))
		return real_access(path, priv_x(amode, 0));
	return real_access(path, amode);
}

int faccessat(int fd, const char *path, int amode, int flag)
{
	const char *v = variant_of("access.");
	int m, f = 0;
	REAL(faccessat);

	if (IS("consts_alt")) {
		if (flag & ~ALT_EACCESS) {
			errno = EINVAL;
			return -1;
		}
		if (flag & ALT_EACCESS)
			f = AT_EACCESS;
		if (alt_amode(amode, &m) != 0)
			return -1;
		if (fd == ALT_FDCWD)
			fd = AT_FDCWD;
		else if (fd == AT_FDCWD && path[0] != '/') {
			errno = EBADF;
			return -1;
		}
		return real_faccessat(fd, path, m, f);
	}
	if (IS("priv_x_ok"))
		return real_faccessat(fd, path, priv_x(amode, (flag & AT_EACCESS) != 0), flag);
	return real_faccessat(fd, path, amode, flag);
}
