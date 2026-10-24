/*
 * lseek.c - conforming variants of lseek(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "whence_values": SEEK_SET, SEEK_CUR, SEEK_END, SEEK_DATA and SEEK_HOLE
 * are 10, 11, 12, 13 and 14 (the header half is lseek.whence_values.h).
 * XBD <stdio.h> and <unistd.h> require only distinct values. This
 * system's own values are then not proper whence values, and XSH lseek()
 * requires EINVAL for them.
 */
static int translate(int whence, int *out)
{
	switch (whence) {
	case 10: *out = SEEK_SET; return 0;
	case 11: *out = SEEK_CUR; return 0;
	case 12: *out = SEEK_END; return 0;
#ifdef SEEK_DATA
	case 13: *out = SEEK_DATA; return 0;
	case 14: *out = SEEK_HOLE; return 0;
#endif
	}
	if (whence == SEEK_SET || whence == SEEK_CUR || whence == SEEK_END
#ifdef SEEK_DATA
	    || whence == SEEK_DATA || whence == SEEK_HOLE
#endif
	    )
		return -1;
	*out = whence;
	return 0;
}

off_t lseek(int fd, off_t off, int whence)
{
	const char *v = variant_of("lseek.");
	int w;
	REAL(lseek);

	if (IS("whence_values")) {
		if (translate(whence, &w) != 0) {
			/* on a closed descriptor EBADF applies too (XSH 2.3: either) */
			if (fcntl(fd, F_GETFD) == -1) {
				errno = EBADF;
				return -1;
			}
			errno = EINVAL;
			return -1;
		}
		return real_lseek(fd, off, w);
	}
	return real_lseek(fd, off, whence);
}

#ifdef __GLIBC__
off_t lseek64(int fd, off_t off, int whence)
{
	return lseek(fd, off, whence);
}
#endif
