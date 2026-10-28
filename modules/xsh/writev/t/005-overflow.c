/*
 * writev() with lengths summing past SSIZE_MAX fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.writev.005
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

	pc_begin("XSH.writev.005");
	pc_make_file("f", "keep");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 2, SEEK_SET) == 2, "lseek failed");
	memset(buf, 'x', sizeof buf);
	iov[0].iov_base = buf; iov[0].iov_len = (size_t)SSIZE_MAX;
	iov[1].iov_base = buf; iov[1].iov_len = 1;
	pc_test();
	expect_overflow_err(writev(fd, iov, 2), "writev() with SSIZE_MAX and 1");
	iov[1].iov_len = (size_t)SSIZE_MAX;
	expect_overflow_err(writev(fd, iov, 2), "writev() with SSIZE_MAX and SSIZE_MAX");
	pc_check(pc_file_equals("f", "keep"), "data was transferred");
	pc_check(lseek(fd, 0, SEEK_CUR) == 2, "the file offset changed");
	pc_pass();
}
