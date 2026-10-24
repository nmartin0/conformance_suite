/*
 * O_TRUNC with O_RDWR truncates.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.046
 * MUTANT    open.trunc_ignored
 * VARIANT   open.accmode_bits
 */
#include "pc.h"
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;

	pc_begin("XSH.open.046");
	pc_make_file("f", "data");
	pc_test();
	fd = open("f", O_RDWR | O_TRUNC);
	pc_expect_ok(fd, "open(\"f\", O_RDWR|O_TRUNC)");
	pc_check(pc_file_size("f") == 0, "size is %ld, expected 0", pc_file_size("f"));
	pc_pass();
}
