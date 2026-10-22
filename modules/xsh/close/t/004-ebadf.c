/*
 * close() of a descriptor that is not open: -1 with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.close.004
 * MUTANT    close.ebadf_as_einval
 * MUTANT    close.always_ok
 */
#include "pc.h"
#include <errno.h>
#include <unistd.h>

int main(void)
{
	int fd;

	pc_begin("XSH.close.004");
	fd = pc_lowest_free_fd();
	pc_test();
	pc_expect_err(close(fd), EBADF, "close(<not open>)");
	pc_pass();
}
