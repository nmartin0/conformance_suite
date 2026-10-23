/*
 * dup() of a closed descriptor gives EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.004
 * MUTANT    dup.ebadf_as_einval
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int bad;

	pc_begin("XSH.dup.004");
	bad = pc_lowest_free_fd();
	pc_test();
	pc_expect_err(dup(bad), EBADF, "dup(<not open>)");
	pc_pass();
}
