/*
 * A failing call sets errno, which then stays.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.errno.006
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>

int main(void)
{
	int fd, e1, e2;

	pc_begin("XSH.errno.006");
	pc_test();
	errno = 0;
	fd = open("no-such-file-for-errno-test", O_RDONLY);
	pc_check(fd == -1, "open() of a missing file returned %d", fd);
	e1 = errno;
	e2 = errno;
	pc_check(e1 == ENOENT, "errno is %s after the failed open(), expected ENOENT", pc_errname(e1));
	pc_check(e2 == e1, "errno changed from %s to %s without a call", pc_errname(e1), pc_errname(e2));
	errno = EXDEV;
	pc_check(errno == EXDEV, "an assignment did not replace the value");
	pc_pass();
}
