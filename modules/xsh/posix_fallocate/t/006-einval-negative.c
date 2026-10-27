/*
 * posix_fallocate() with a negative offset or len gives EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_fallocate.006
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* File "f" holding n bytes of c; a descriptor open O_RDWR. */
static int mkf(size_t n, int c)
{
	char buf[128];
	int fd;

	memset(buf, c, sizeof buf);
	fd = open("f", O_RDWR | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		pc_unresolved("cannot create f: %s", pc_errname(errno));
	while (n > 0) {
		size_t k = n < sizeof buf ? n : sizeof buf;

		if (write(fd, buf, k) != (ssize_t)k)
			pc_unresolved("cannot fill f: %s", pc_errname(errno));
		n -= k;
	}
	return fd;
}

/* The file system may refuse the operation (ENOTSUP); then the test does not apply. */
static void check_supported(int r)
{
	if (r == ENOTSUP)
		pc_notinuse("the file system does not support posix_fallocate() (ENOTSUP)");
	if (r == ENOSPC || r == EIO || r == EINTR)
		pc_unresolved("posix_fallocate() returned %s", pc_errname(r));
}


int main(void)
{
	int fd, r;

	pc_begin("XSH.posix_fallocate.006");
	fd = mkf(10, 'x');
	pc_test();
	r = posix_fallocate(fd, -1, 10);
	if (r == ENOTSUP)
		pc_notinuse("the file system answered ENOTSUP, which XSH 2.3 also allows here");
	pc_check(r == EINVAL, "posix_fallocate() with offset -1 returned %s", pc_errname(r));
	r = posix_fallocate(fd, 0, -1);
	if (r == ENOTSUP)
		pc_notinuse("the file system answered ENOTSUP, which XSH 2.3 also allows here");
	pc_check(r == EINVAL, "posix_fallocate() with len -1 returned %s", pc_errname(r));
	pc_pass();
}
