/*
 * After close(), the descriptor is not open.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.close.003
 * MUTANT    close.noop
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;

	pc_begin("XSH.close.003");
	pc_make_file("f", "x");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "open");
	pc_test();
	pc_expect_ok(close(fd), "close");
	pc_expect_err(fcntl(fd, F_GETFD), EBADF, "fcntl(F_GETFD) after close");
	pc_pass();
}
