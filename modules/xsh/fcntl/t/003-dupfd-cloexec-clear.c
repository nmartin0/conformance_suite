/*
 * F_DUPFD clears FD_CLOEXEC.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.003
 * MUTANT    fcntl.dupfd_keeps_cloexec
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd, d, f;

	pc_begin("XSH.fcntl.003");
	pc_make_file("f", "x");
	fd = open("f", O_RDONLY | O_CLOEXEC);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	d = fcntl(fd, F_DUPFD, 0);
	pc_expect_ok(d, "fcntl(F_DUPFD, 0)");
	f = fcntl(d, F_GETFD);
	pc_check(f != -1 && !(f & FD_CLOEXEC), "FD_CLOEXEC set on the new descriptor");
	pc_pass();
}
