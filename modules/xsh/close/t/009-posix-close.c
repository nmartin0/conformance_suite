/*
 * posix_close(fd, 0) closes the descriptor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.close.009
 * MUTANT    close.pc_noop
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;

	pc_begin("XSH.close.009");
	pc_make_file("f", "x");
#ifndef POSIX_CLOSE_RESTART
	(void)fd;
	pc_unresolved("<unistd.h> does not define POSIX_CLOSE_RESTART (posix_close() missing)");
#else
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	pc_expect_ok(posix_close(fd, 0), "posix_close(fd, 0)");
	pc_expect_err(fcntl(fd, F_GETFD), EBADF, "fcntl(F_GETFD) after posix_close()");
	pc_pass();
#endif
}
