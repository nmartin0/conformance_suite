/*
 * O_CREAT creates a missing file as a regular file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.001
 * MUTANT    open.creat_ignored
 * MUTANT    open.creat_makes_dir
 * VARIANT   open.accmode_bits
 */
#include "pc.h"
#include <fcntl.h>
#include <sys/stat.h>

int main(void)
{
	struct stat st;
	int fd;

	pc_begin("XSH.open.001");
	pc_require_space();
	pc_test();
	fd = open("new", O_WRONLY | O_CREAT, 0600);
	pc_expect_ok(fd, "open(\"new\", O_WRONLY|O_CREAT)");
	pc_check(lstat("new", &st) == 0, "file \"new\" does not exist");
	pc_check(S_ISREG(st.st_mode), "\"new\" is not a regular file");
	pc_pass();
}
