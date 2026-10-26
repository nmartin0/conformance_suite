/*
 * strerror() of a valid error number leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strerror.002
 * MUTANT    strerror.errno_clobbered
 * VARIANT   strerror.global_buf
 */
#include "pc.h"
#include <errno.h>
#include <string.h>

static const int valid[] = {
	E2BIG, EACCES, EAGAIN, EBADF, EBUSY, ECHILD, EDOM, EEXIST, EFAULT, EILSEQ,
	EINTR, EINVAL, EIO, EISDIR, EMFILE, ENOENT, ENOMEM, ENOSPC, ENOSYS, ENOTDIR,
	EOVERFLOW, EPERM, EPIPE, ERANGE, ESRCH, ETIMEDOUT, EXDEV,
};
#define NVALID (sizeof valid / sizeof valid[0])

int main(void)
{
	size_t i;
	int e;

	pc_begin("XSH.strerror.002");
	pc_test();
	for (i = 0; i < NVALID; i++) {
		errno = 0;
		(void)strerror(valid[i]);
		e = errno;
		pc_check(e == 0, "strerror(%s) changed errno from 0 to %s", pc_errname(valid[i]), pc_errname(e));
	}
	pc_pass();
}
