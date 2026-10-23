/*
 * pwrite() with a negative offset gives EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.024
 * MUTANT    write.pwrite_neg_ok
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	int fd;

	pc_begin("XSH.write.024");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 4, SEEK_SET) == 4, "lseek failed");
	pc_test();
	pc_expect_err(pwrite(fd, "x", 1, -1), EINVAL, "pwrite() at offset -1");
	pc_check(pc_file_equals("f", "0123456789"), "the file changed");
	pc_check(lseek(fd, 0, SEEK_CUR) == 4, "the file offset changed");
	pc_pass();
}
