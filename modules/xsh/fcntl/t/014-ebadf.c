/*
 * fcntl() on a closed descriptor gives EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.014
 * MUTANT    fcntl.ebadf_as_einval
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int bad;

	pc_begin("XSH.fcntl.014");
	bad = pc_lowest_free_fd();
	pc_test();
	pc_expect_err(fcntl(bad, F_GETFD), EBADF, "fcntl(<not open>, F_GETFD)");
	pc_expect_err(fcntl(bad, F_GETFL), EBADF, "fcntl(<not open>, F_GETFL)");
	pc_expect_err(fcntl(bad, F_DUPFD, 0), EBADF, "fcntl(<not open>, F_DUPFD)");
	pc_pass();
}
