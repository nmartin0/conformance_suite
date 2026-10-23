/*
 * F_DUPFD_CLOEXEC sets FD_CLOEXEC.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.004
 * MUTANT    fcntl.dupfd_cloexec_ignored
 * MUTANT    fcntl.dupfd_ignores_arg
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd, arg, d, f;

	pc_begin("XSH.fcntl.004");
	pc_make_file("f", "x");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	arg = pc_lowest_free_fd() + 3;
	pc_check(fcntl(arg, F_GETFD) == -1, "slot %d is not free", arg);
	pc_test();
	d = fcntl(fd, F_DUPFD_CLOEXEC, arg);
	pc_check(d == arg, "F_DUPFD_CLOEXEC with arg %d returned %d", arg, d);
	f = fcntl(d, F_GETFD);
	pc_check(f != -1 && (f & FD_CLOEXEC), "FD_CLOEXEC clear on the new descriptor");
	pc_pass();
}
