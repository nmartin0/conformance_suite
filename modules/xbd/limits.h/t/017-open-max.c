/*
 * A process can have descriptors 0 up to {OPEN_MAX} - 1 open at once.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.017
 */
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include "pc.h"

int main(void)
{
	long n;
	int fd, last = -1;

	pc_begin("XBD.limits.h.017");
	pc_make_file("f", "x");
	n = sysconf(_SC_OPEN_MAX);
	pc_test();
	pc_check(n == -1 || n >= _POSIX_OPEN_MAX,
	    "sysconf(_SC_OPEN_MAX) is %ld, below the minimum %d", n, _POSIX_OPEN_MAX);

	/* Open until descriptor number _POSIX_OPEN_MAX - 1 has been handed out. */
	for (;;) {
		fd = open("f", O_RDONLY);
		if (fd == -1)
			pc_fail("open() failed with %s after descriptor %d; descriptors 0 to %d must all be usable",
			    pc_errname(errno), last, _POSIX_OPEN_MAX - 1);
		last = fd;
		if (fd >= _POSIX_OPEN_MAX - 1)
			break;
	}

	/* Up to a modest reported limit, the last descriptor is n - 1 and n fails. */
	if (n > _POSIX_OPEN_MAX && n <= 4096) {
		while (last < n - 1) {
			fd = open("f", O_RDONLY);
			if (fd == -1)
				pc_fail("open() failed with %s at descriptor %d; sysconf(_SC_OPEN_MAX) is %ld",
				    pc_errname(errno), last + 1, n);
			last = fd;
		}
		pc_check(last == n - 1, "descriptor %d was assigned; the limit is %ld", last, n);
		fd = open("f", O_RDONLY);
		pc_expect_err(fd, EMFILE, "open() with every descriptor in use");
	}
	pc_pass();
}
