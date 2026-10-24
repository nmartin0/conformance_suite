/*
 * O_CREAT|O_EXCL on an existing file: -1/EEXIST, file untouched.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.003
 * MUTANT    open.excl_ignored
 * MUTANT    open.eexist_as_eacces
 * MUTANT    open.excl_truncates
 * VARIANT   open.accmode_bits
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>

int main(void)
{
	int fd;

	pc_begin("XSH.open.003");
	pc_make_file("f", "keep");
	pc_test();
	fd = open("f", O_WRONLY | O_CREAT | O_EXCL, 0600);
	pc_expect_err(fd, EEXIST, "open(\"f\", O_WRONLY|O_CREAT|O_EXCL)");
	pc_check(pc_file_equals("f", "keep"), "existing file was modified");
	pc_pass();
}
