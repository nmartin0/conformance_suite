/*
 * writev() with an invalid iovcnt fails with EINVAL or is carried out.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.writev.007
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
	struct iovec one, *many = NULL;
	long max;
	ssize_t r;
	int fd, failed = 0, i;

	pc_begin("XSH.writev.007");
	pc_require_space();
	fd = open("f", O_WRONLY | O_CREAT, 0600);
	pc_check(fd >= 0, "cannot create f");
	one.iov_base = "abcdefgh"; one.iov_len = 8;
#ifdef IOV_MAX
	max = IOV_MAX;
#else
	max = sysconf(_SC_IOV_MAX);
#endif
	if (max > 0 && max < 1000000) {
		many = malloc(((size_t)max + 1) * sizeof *many);
		pc_check(many != NULL, "malloc failed");
		for (i = 0; i <= max; i++) {
			many[i].iov_base = "x";
			many[i].iov_len = 1;
		}
	}
	pc_test();
	r = writev(fd, &one, 0);
	if (r == -1) {
		pc_check(errno == EINVAL, "writev() with iovcnt 0 failed with %s, not EINVAL", pc_errname(errno));
		failed = 1;
	}
	if (many != NULL) {
		r = writev(fd, many, (int)(max + 1));
		if (r == -1) {
			pc_check(errno == EINVAL, "writev() with iovcnt {IOV_MAX}+1 failed with %s, not EINVAL", pc_errname(errno));
			failed = 1;
		}
	}
	if (!failed)
		pc_notinuse("the system carried out the calls with an invalid iovcnt");
	pc_pass();
}
