/*
 * write() on a closed or read-only descriptor gives EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.019
 * MUTANT    write.ebadf_as_einval
 * MUTANT    write.rdonly_ok
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	int bad, r;

	pc_begin("XSH.write.019");
	pc_make_file("f", "x");
	r = open("f", O_RDONLY);
	pc_check(r >= 0, "cannot open f");
	bad = pc_lowest_free_fd();
	pc_test();
	pc_expect_err(write(bad, "y", 1), EBADF, "write(<not open>)");
	pc_expect_err(write(r, "y", 1), EBADF, "write(<read-only descriptor>)");
	pc_check(pc_file_equals("f", "x"), "the file was written");
	pc_pass();
}
