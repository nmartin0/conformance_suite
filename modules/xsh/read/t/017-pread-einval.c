/*
 * pread() with a negative offset gives EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.017
 * MUTANT    read.pread_neg_ok
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	char c;
	int fd;

	pc_begin("XSH.read.017");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 4, SEEK_SET) == 4, "lseek failed");
	pc_test();
	pc_expect_err(pread(fd, &c, 1, -1), EINVAL, "pread() at offset -1");
	pc_check(lseek(fd, 0, SEEK_CUR) == 4, "the file offset changed");
	pc_pass();
}
