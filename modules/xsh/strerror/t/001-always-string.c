/*
 * strerror() always returns a pointer to a string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strerror.001
 * MUTANT    strerror.null_invalid
 * MUTANT    strerror.null_zero
 * VARIANT   strerror.einval_empty
 * VARIANT   strerror.zero_empty
 * VARIANT   strerror.global_buf
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <string.h>

static const int nums[] = {
	E2BIG, EACCES, EAGAIN, EBADF, EBUSY, ECHILD, EDOM, EEXIST, EFAULT, EILSEQ,
	EINTR, EINVAL, EIO, EISDIR, EMFILE, ENOENT, ENOMEM, ENOSPC, ENOSYS, ENOTDIR,
	EOVERFLOW, EPERM, EPIPE, ERANGE, ESRCH, ETIMEDOUT, EXDEV,
	0, -1, INT_MIN, INT_MAX,
};

int main(void)
{
	size_t i;
	volatile size_t sink = 0;

	pc_begin("XSH.strerror.001");
	pc_test();
	for (i = 0; i < sizeof nums / sizeof nums[0]; i++) {
		const char *s = strerror(nums[i]);

		pc_check(s != NULL, "strerror(%d) returned a null pointer", nums[i]);
		/* Reading to the terminator: a missing one shows as a fault. */
		sink += strlen(s);
	}
	pc_pass();
}
