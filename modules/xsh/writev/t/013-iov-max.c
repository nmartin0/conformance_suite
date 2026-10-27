/*
 * writev() with iovcnt equal to {IOV_MAX} is valid and succeeds.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.writev.013
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/uio.h>

int main(void)
{
	char *data;
	struct iovec *iov;
	long max;
	ssize_t r;
	int fd, i;

	pc_begin("XSH.writev.013");
#ifdef IOV_MAX
	max = IOV_MAX;
#else
	max = sysconf(_SC_IOV_MAX);
#endif
	if (max <= 0 || max >= 1000000)
		pc_notinuse("{IOV_MAX} is indeterminate or too large here (%ld)", max);
	data = malloc((size_t)max + 1);
	iov = malloc((size_t)max * sizeof *iov);
	pc_check(data != NULL && iov != NULL, "malloc failed");
	memset(data, 'k', (size_t)max);
	data[max] = '\0';
	pc_require_space();
	fd = open("f", O_WRONLY | O_CREAT, 0600);
	pc_check(fd >= 0, "cannot create f");
	for (i = 0; i < max; i++) {
		iov[i].iov_base = data + i;
		iov[i].iov_len = 1;
	}
	pc_test();
	r = writev(fd, iov, (int)max);
	pc_check(r != -1, "writev() with iovcnt {IOV_MAX} failed: %s", pc_errname(errno));
	pc_check(r == max, "writev() returned %ld, expected %ld", (long)r, max);
	pc_check(pc_file_equals("f", data), "the file does not hold the bytes written");
	pc_pass();
}
