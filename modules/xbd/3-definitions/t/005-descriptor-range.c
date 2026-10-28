/*
 * New file descriptors are non-negative and below {OPEN_MAX}.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.3-definitions.005
 */
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "pc.h"

int main(void)
{
	long om;
	int fd, fd2, p[2];

	pc_begin("XBD.3-definitions.005");
	errno = 0;
	om = sysconf(_SC_OPEN_MAX);
	if (om < 0)
		pc_notinuse("sysconf(_SC_OPEN_MAX) reports no limit");
	pc_make_file("f", "x");
	pc_test();
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0 && fd < om, "open returned descriptor %d; "
	    "sysconf(_SC_OPEN_MAX) is %ld", fd, om);
	fd2 = dup(fd);
	pc_check(fd2 >= 0 && fd2 < om, "dup returned descriptor %d; "
	    "sysconf(_SC_OPEN_MAX) is %ld", fd2, om);
	pc_check(pipe(p) == 0, "pipe failed: %s", pc_errname(errno));
	pc_check(p[0] >= 0 && p[0] < om && p[1] >= 0 && p[1] < om,
	    "pipe returned descriptors %d and %d; sysconf(_SC_OPEN_MAX) is %ld",
	    p[0], p[1], om);
	pc_pass();
}
