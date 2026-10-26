/*
 * strerror() of a valid error number gives a non-empty message.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strerror.003
 * MUTANT    strerror.empty_valid
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
	const char *s;

	pc_begin("XSH.strerror.003");
	pc_test();
	for (i = 0; i < NVALID; i++) {
		s = strerror(valid[i]);
		pc_check(s != NULL, "strerror(%s) returned a null pointer", pc_errname(valid[i]));
		pc_check(s[0] != '\0', "strerror(%s) returned an empty string", pc_errname(valid[i]));
	}
	pc_pass();
}
