/*
 * F_GETFL returns the access mode and status flags.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.009
 * MUTANT    fcntl.getfl_no_accmode
 * MUTANT    fcntl.getfl_no_append
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd, fl;

	pc_begin("XSH.fcntl.009");
	pc_make_file("f", "x");
	pc_test();
	fd = open("f", O_RDWR | O_APPEND);
	pc_expect_ok(fd, "open(\"f\", O_RDWR|O_APPEND)");
	fl = fcntl(fd, F_GETFL);
	pc_check(fl >= 0, "F_GETFL returned %d", fl);
	pc_check((fl & O_ACCMODE) == O_RDWR, "access mode %#x, expected O_RDWR", fl & O_ACCMODE);
	pc_check(fl & O_APPEND, "O_APPEND not shown");
	pc_pass();
}
