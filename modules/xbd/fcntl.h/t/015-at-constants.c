/*
 * AT_FDCWD is not the number of any descriptor (it differs from the
 * descriptors in use and is negative or beyond OPEN_MAX), and AT_EACCESS,
 * AT_SYMLINK_NOFOLLOW, AT_SYMLINK_FOLLOW and AT_REMOVEDIR are nonzero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.015
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	pc_begin("XBD.fcntl.h.015");
#if !defined(AT_FDCWD) || !defined(AT_EACCESS) || !defined(AT_SYMLINK_NOFOLLOW) || !defined(AT_SYMLINK_FOLLOW) || !defined(AT_REMOVEDIR)
	pc_unresolved("an AT_ constant is missing (the header check reports it)");
#else
	int fd, fd2;
	long openmax;

	pc_make_dir("d");
	fd = open("d", O_RDONLY);
	if (fd == -1)
		pc_unresolved("open failed (%s)", pc_errname(errno));
	fd2 = dup(fd);
	if (fd2 == -1)
		pc_unresolved("dup failed (%s)", pc_errname(errno));
	openmax = sysconf(_SC_OPEN_MAX);
	pc_test();
	/* A value used in place of a descriptor must not be one. */
	pc_check(AT_FDCWD != fd && AT_FDCWD != fd2 && AT_FDCWD != 0 && AT_FDCWD != 1 && AT_FDCWD != 2,
	    "AT_FDCWD (%d) is the number of a descriptor in use", (int)AT_FDCWD);
	pc_check(AT_FDCWD < 0 || (openmax != -1 && (long)AT_FDCWD >= openmax),
	    "AT_FDCWD (%d) is in the range of descriptor numbers", (int)AT_FDCWD);
	/* A flag argument of zero means no flag, so each flag differs from zero. */
	pc_check(AT_EACCESS != 0, "AT_EACCESS is zero");
	pc_check(AT_SYMLINK_NOFOLLOW != 0, "AT_SYMLINK_NOFOLLOW is zero");
	pc_check(AT_SYMLINK_FOLLOW != 0, "AT_SYMLINK_FOLLOW is zero");
	pc_check(AT_REMOVEDIR != 0, "AT_REMOVEDIR is zero");
	pc_pass();
#endif
}
