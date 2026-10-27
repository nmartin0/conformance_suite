/*
 * readv() with an invalid iovcnt fails with EINVAL or is carried out.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.readv.011
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

int main(void)
{
	char buf[16];
	struct iovec one, *many = NULL;
	long max;
	ssize_t r;
	int fd, failed = 0, i;

	pc_begin("XSH.readv.011");
	pc_make_file("f", "0123456789abcdef0123456789abcdef");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	one.iov_base = buf; one.iov_len = sizeof buf;
#ifdef IOV_MAX
	max = IOV_MAX;
#else
	max = sysconf(_SC_IOV_MAX);
#endif
	if (max > 0 && max < 1000000) {
		many = malloc(((size_t)max + 1) * sizeof *many);
		pc_check(many != NULL, "malloc failed");
		for (i = 0; i <= max; i++) {
			many[i].iov_base = buf;
			many[i].iov_len = 1;
		}
	}
	pc_test();
	r = readv(fd, &one, 0);
	if (r == -1) {
		pc_check(errno == EINVAL, "readv() with iovcnt 0 failed with %s, not EINVAL", pc_errname(errno));
		failed = 1;
	}
	if (many != NULL) {
		r = readv(fd, many, (int)(max + 1));
		if (r == -1) {
			pc_check(errno == EINVAL, "readv() with iovcnt {IOV_MAX}+1 failed with %s, not EINVAL", pc_errname(errno));
			failed = 1;
		}
	}
	if (!failed)
		pc_notinuse("the system carried out the calls with an invalid iovcnt");
	pc_pass();
}
