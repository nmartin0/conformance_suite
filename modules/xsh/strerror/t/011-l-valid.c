/*
 * strerror_l() of a valid number or 0: a string, errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strerror.011
 * MUTANT    strerror.l_null
 * MUTANT    strerror.l_errno_clobbered
 * MUTANT    strerror.l_empty
 * VARIANT   strerror.zero_empty
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <string.h>

static const int valid[] = {
	E2BIG, EACCES, EAGAIN, EBADF, EBUSY, ECHILD, EDOM, EEXIST, EFAULT, EILSEQ,
	EINTR, EINVAL, EIO, EISDIR, EMFILE, ENOENT, ENOMEM, ENOSPC, ENOSYS, ENOTDIR,
	EOVERFLOW, EPERM, EPIPE, ERANGE, ESRCH, ETIMEDOUT, EXDEV, 0,
};

int main(void)
{
	locale_t loc;
	const char *s;
	size_t i;
	int e;

	pc_begin("XSH.strerror.011");
	loc = newlocale(LC_ALL_MASK, "POSIX", (locale_t)0);
	pc_check(loc != (locale_t)0, "newlocale(POSIX) failed: %s", pc_errname(errno));
	pc_test();
	for (i = 0; i < sizeof valid / sizeof valid[0]; i++) {
		errno = 0;
		s = strerror_l(valid[i], loc);
		e = errno;
		pc_check(s != NULL, "strerror_l(%d) returned a null pointer (errno %s)", valid[i], pc_errname(e));
		pc_check(e == 0, "strerror_l(%d) changed errno from 0 to %s", valid[i], pc_errname(e));
		pc_check(valid[i] == 0 || s[0] != '\0', "strerror_l(%s) returned an empty string", pc_errname(valid[i]));
	}
	pc_pass();
}
