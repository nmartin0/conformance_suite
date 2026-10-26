/*
 * strerror_r() with a large buffer stores the message and returns 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strerror.008
 * MUTANT    strerror.r_empty
 * MUTANT    strerror.r_unterminated
 * MUTANT    strerror.r_zero_einval
 * VARIANT   strerror.zero_empty
 * VARIANT   strerror.r_errno
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

static char buf[65536];

/* strerror_r(e) into a buffer full of 'X'; FAIL unless it returns 0 with a terminated string. */
static void one(int e, int nonempty)
{
	int r;

	memset(buf, 'X', sizeof buf);
	r = strerror_r(e, buf, sizeof buf);
	if (r == ERANGE)
		pc_unresolved("strerror_r(%d) gave ERANGE with a %zu-byte buffer", e, sizeof buf);
	pc_check(r == 0, "strerror_r(%d) returned %d (%s), expected 0", e, r, pc_errname(r));
	pc_check(memchr(buf, '\0', sizeof buf) != NULL, "strerror_r(%d) stored no null byte", e);
	if (nonempty)
		pc_check(buf[0] != '\0', "strerror_r(%s) stored an empty string", pc_errname(e));
}

int main(void)
{
	size_t i;

	pc_begin("XSH.strerror.008");
	pc_test();
	for (i = 0; i < NVALID; i++)
		one(valid[i], 1);
	one(0, 0);
	pc_pass();
}
