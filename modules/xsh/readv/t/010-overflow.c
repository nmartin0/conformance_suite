/*
 * readv() with lengths summing past SSIZE_MAX fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.readv.010
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/uio.h>

/*
 * The buffers described are far larger than any storage there is, so an
 * invalid address may be detected too; with two errors present any one may
 * be returned (XSH 2.3). The overflow error is the one asserted, EFAULT is
 * the other that is tolerated.
 */
static void expect_overflow_err(long r, const char *what)
{
	int e = errno;

	pc_check(r == -1, "%s: returned %ld, expected -1 with EINVAL", what, r);
	pc_check(e == EINVAL || e == EFAULT, "%s: errno %s, expected EINVAL (or EFAULT for the invalid buffers)", what, pc_errname(e));
}

int main(void)
{
	char buf[16];
	struct iovec iov[2];
	int fd;

	pc_begin("XSH.readv.010");
	pc_make_file("f", "abc");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	iov[0].iov_base = buf; iov[0].iov_len = (size_t)SSIZE_MAX;
	iov[1].iov_base = buf; iov[1].iov_len = (size_t)SSIZE_MAX;
	pc_test();
	expect_overflow_err(readv(fd, iov, 2), "readv() with two buffers of SSIZE_MAX");
	pc_pass();
}
