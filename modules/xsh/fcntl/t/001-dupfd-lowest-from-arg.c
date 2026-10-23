/*
 * F_DUPFD returns the lowest free descriptor at or above arg.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.001
 * MUTANT    fcntl.dupfd_ignores_arg
 * MUTANT    fcntl.dupfd_not_lowest
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd, low, arg, r;

	pc_begin("XSH.fcntl.001");
	pc_make_file("f", "x");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	low = pc_lowest_free_fd();
	arg = low + 2;
	pc_check(fcntl(fd, F_DUPFD, arg) == arg, "cannot place a descriptor at %d", arg);
	pc_check(fcntl(low, F_GETFD) == -1 && fcntl(arg + 1, F_GETFD) == -1,
	    "slots %d and %d are not free", low, arg + 1);
	pc_test();
	r = fcntl(fd, F_DUPFD, arg);
	pc_check(r == arg + 1, "F_DUPFD with arg %d returned %d, expected %d", arg, r, arg + 1);
	pc_pass();
}
